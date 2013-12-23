/*****
** crawler.c
** - implements the methods declared in crawler.h
** -爬取线程的核心流程
*/
#include "crawler.h"
#include "http_client.h"
#include "link_parser.h"
#include "network.h"
#include "tpool.h"
#include <stdlib.h>

void* do_crawler(void *arg) //完成爬取线程的核心流程
{
	struct para_for_crawler *para = (struct para_for_crawler *)arg;
	struct web_graph *webg = para->webg;
	urlq_t *queue = para->queue;
	tpool_t *tpool = para->tpool;

	int client_fd = -1;
	char *http_response = NULL;
	char **url_list = NULL;
	int url_list_size = 0;
	int i = 0, j;
	int num = 0, cur_num = 0;
	int *out_link = NULL;//存放当前这个url的边集（这个数组里面存放的是目标顶点编号）
	char *cur_url = NULL;
	int try_times = 0;//获取网页失败之后，尝试3次，如果3次都还是获取不了，就说明不行，放弃尝试
	char *cur_path = NULL;

	url_list = (char **)malloc(sizeof (char *) * URL_LIST_LEN);
	for (i = 0; i < URL_LIST_LEN; i++)
		url_list[i] = (char *)malloc(sizeof (char) * LINK_LEN);//表大小为5000*512

	cur_url = (char *)malloc(sizeof (char ) * LINK_LEN);
	cur_path = (char *)malloc(sizeof (char ) * LINK_LEN);

	while (1)
	{
		/*如果线程池没有被销毁且没有任务要执行，则挂起线程*/
		pthread_mutex_lock(&tpool->lock);
		while (queue->size == 0 && tpool->shutdown == 0)
		{
			tpool->at_work--;
			pthread_cond_wait(&tpool->cond, &tpool->lock);//挂起前会解锁,唤醒后会重新加锁
			tpool->at_work++;
		}
		if (tpool->shutdown)
		{
			tpool->at_work--;
			pthread_mutex_unlock(&tpool->lock);
			pthread_exit(NULL);
		}
		pthread_mutex_unlock(&tpool->lock);

		/*这里需要上锁，因为需要对队列进行改动*/
		pthread_mutex_lock(&tpool->lock);
		cur_num = queue_pop(queue);
		pthread_mutex_unlock(&tpool->lock);
		/*取得队列中的url后解锁*/

		/*因为调试的时候，发现cur_num超出了all_url_list_size的范围，所以加个这个判断，跳过这个bug*/
		if (cur_num > webg->all_url_list_size)
		{
			printf("-----------------------cur_num over url list size!---------------------------------\n");
			continue;
		}
		if (cur_num == -1)
		{
			printf("--------------------empty_queue!-------------------\n");
			continue;
		}
		/*从图中根据编号获得url的具体内容*/
		strcpy(cur_url, webg->all_url_list[cur_num]);
		if (strlen(cur_url) >= LINK_LEN)
		{
			printf("fail:::too long link!\n len: %d\n", strlen(cur_url));
			printf("long url: %s\n", cur_url);
			continue;
		}

		for (i = strlen(cur_url) - 1; i >= 0; i--)
			if (cur_url[i] == '/')
				break;
		for (j = 0; j <= i; j++)
			cur_path[j] = cur_url[j];
		cur_path[i+1] = '\0';

		/*这部分工作不需要锁，不是临界资源*/
		
		/*根据url获取网页内容*/
		//一共有3次尝试机会
		try_times = 3;
		http_response = NULL;
		while (try_times != 0 && http_response == NULL)
		{
			client_fd = open_tcp(SITE_IP, SITE_PORT);
			if (client_fd < 0)
			{
				printf("fail to open tcp!\n");
				try_times--;
				continue;
			}
			http_response = http_do_get(client_fd, cur_url);
			try_times--;
			close(client_fd);
		}
		/*client_fd = open_tcp(SITE_IP, SITE_PORT);
		if (client_fd < 0)
		{
			printf("fail to open tcp!\n");
			continue;
		}
		http_response = http_do_get(client_fd, cur_url);
		close(client_fd);*/

		//3次尝试机会还是为NULL，那就说明这个网页应该是抓不到了，不管它了
		if (http_response == NULL || strlen(http_response) < 10)//接收超时了, 或者是接收回来的长度小于10，遇到过接收回来长度为0
		{
			printf("fail to fetch this page: %s\n", cur_url);
			continue;
		}
		
		if (http_response_status(http_response) == 200)
		{
			webg->existed_page[cur_num] = 1;
			pthread_mutex_lock(&tpool->lock);
			webg->existed_page_size++;
			pthread_mutex_unlock(&tpool->lock);

			url_list_size = extract_link(http_response, url_list, cur_url, cur_path);//获得url_list及其大小
			if (url_list_size > 4000)
			{
				printf("url_list_size > 4000!\nurl_list_size: %d\n", url_list_size);
				free(http_response);
				continue;
			}
			//将url_list中的url放入点集
			out_link = (int *)malloc(sizeof (int ) * url_list_size);
			if (out_link == NULL)
			{
				printf("malloc out_link fail!\n");
				continue;
			}

			/*这段涉及到对队列和图的修改，所以需要上锁*/
			for (i = 0; i < url_list_size; i++)
			{
				if (strlen(url_list[i]) >= LINK_LEN)
				{
					printf("-----------fail::too long url!\n url_len: %d\nurl: %s\n-----------\n", strlen(url_list[i]), url_list[i]);
					i++;
					continue;
				}
				pthread_mutex_lock(&tpool->lock);
				if ((num = has_vertex(webg, url_list[i])) == NOT_IN_SET)//表示这个url不在点集中，这里还需要添加将这个url入队列
				{
					num = insert_vertex(webg, url_list[i]);
					if (num == -1)
						printf("fail to insert!\n");
					if (num > 400000)//防止错误的数插到队列里面去
					{
						printf("fail: wrong number to be inserted into the graph\n");
						i++;
						pthread_mutex_unlock(&tpool->lock);
						continue;
					}
					queue_push(queue, num);
					if (tpool->at_work < tpool->max_thr_num)//这里特别重要，不是什么时候都能signal的，如果线程都在工作的话，这个signal就会丢失
						pthread_cond_signal(&tpool->cond);
				}
				pthread_mutex_unlock(&tpool->lock);
				out_link[i] = num;//将目标顶点的编号存放到这个url的边集里面去
			}
		
			/*因为每个线程所要处理的url不同，即cur_url都不同，所以对边集的操作就不同，所以这里对边集的操作不需要加锁*/
			num = remove_duplicate(out_link, url_list_size);//返回去重后的顶点个数 
			webg->edge_set[cur_num] = (int *)malloc(sizeof (int ) * (num + 1));//给当前url的边集分配空间
			if (webg->edge_set[cur_num] == NULL)
			{
				printf("-----------------------------fail to malloc space for edge set for %d  url\n", cur_num);
				free(out_link);
				continue;
			}
			webg->edge_set[cur_num][0] = num;
			for (i = 0; i < num; i++)
			{
				pthread_mutex_lock(&tpool->lock);
				webg->ind[out_link[i]]++;//统计入度
				webg->edge_set_size++;
				pthread_mutex_unlock(&tpool->lock);

				insert_edge(webg, cur_num, out_link[i], i+1);//i+1是因为这个边集数组里面，第0号已经存了出度了
			}

			//释放edge占用的内存
			free(out_link);
			out_link = NULL;
		}
		free(http_response);//这里记得将http_response释放掉，这个很耗内存的
		http_response = NULL;
	}

	//释放url_list所占用的内存
	for (i = 0; i < URL_LIST_LEN; i++)
		free(url_list[i]);
	free(url_list);
	url_list = NULL;
	free(cur_url);
}

int remove_duplicate(int *out_link, int size)
{
	int cur, cur_pos, i;
	
	qsort(out_link, size, sizeof (int ), comp);
	cur = -1;//因为点的编号不可能存在-1
	cur_pos = -1;
	for (i = 0; i < size; i++)
	{
		if (out_link[i] != cur)
		{
			cur_pos++;
			cur = out_link[i];
			out_link[cur_pos] = cur;
		}
	}
	return cur_pos + 1;
}

int comp(const void *a,const void *b)
{
	return *(int *)a-*(int *)b;
}
