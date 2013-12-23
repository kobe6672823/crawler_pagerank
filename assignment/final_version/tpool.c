#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#include "tpool.h"
#include "crawler.h"

static tpool_t *tpool = NULL;

tpool_t* tpool_create(int max_thr_num, struct web_graph *webg, urlq_t *queue)
{
	int i;
	struct para_for_crawler *para = NULL;

	/*初始化*/
	tpool = calloc(1, sizeof (tpool_t));
	if (tpool == NULL)
	{
		printf("创建线程池失败！\n");
		return NULL;
	}

	tpool->max_thr_num = max_thr_num;
	tpool->shutdown = 0;
	tpool->at_work = max_thr_num;
	if (pthread_mutex_init(&tpool->lock, NULL) != 0)
	{
		printf("初始化互斥锁失败！\n");
		return NULL;
	}
	if (pthread_cond_init(&tpool->cond, NULL) != 0)
	{
		printf("初始化线程条件变量失败！\n");
		return NULL;
	}

	/*创建线程*/
	tpool->thr_id = calloc(max_thr_num, sizeof (pthread_t));
	if (tpool->thr_id == NULL)
	{
		printf("为线程id数组分配空间失败！\n");
		return NULL;
	}

	para = (struct para_for_crawler *)malloc(sizeof (struct para_for_crawler));
	para->tpool = tpool;
	para->webg = webg;
	para->queue = queue;
	for (i = 0; i < max_thr_num; i++)
	{
		if (pthread_create(&tpool->thr_id[i], NULL, do_crawler, para) != 0)//这里还需要传参给do_crawler
		{
			printf("创建线程失败!\n");
			return NULL;
		}
	}

	return tpool;
}

void tpool_destroy()
{
	int i;
	
	if (tpool->shutdown)
		return;
	tpool->shutdown = 1;

	/*通知所有正在等待的线程*/
	pthread_mutex_lock(&tpool->lock);
	pthread_cond_broadcast(&tpool->cond);
	pthread_mutex_unlock(&tpool->lock);

	for (i = 0; i < tpool->max_thr_num; i++)
		pthread_join(tpool->thr_id[i], NULL);
	free(tpool->thr_id);

	pthread_mutex_destroy(&tpool->lock);
	pthread_cond_destroy(&tpool->cond);

	free(tpool);
}
