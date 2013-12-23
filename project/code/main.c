/*****
** main.c
** - 主线程，维持线程池，线程间通信等
*/
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "crawler.h"
#include "urlqueue.h"
#include "webgraph.h"
#include "tpool.h"

#define MAX_THREADS 200

int main(int argc, char *argv[])
{
	struct web_graph webg;
	int i;
	urlq_t queue;
	int num;
	tpool_t *tpool;

	if (init_webg(&webg) == INIT_FAIL)
	{
		printf("初始化图失败，退出程序！\n");
		exit(1);
	}
	if (queue_init(&queue) == INIT_QUEUE_FAIL)
	{
		printf("初始化队列失败，退出程序！\n");
		exit(1);
	}

	if ((tpool = tpool_create(MAX_THREADS, &webg, &queue)) == NULL)
	{
		printf("初始化线程池失败！\n");
		exit(1);
	}
	printf("start work!\n");
	//首先要将index.html加入点集和队列中
	pthread_mutex_lock(&tpool->lock);
	num = insert_vertex(&webg, "/techqq/index.html");
	queue_push(&queue, num);
	pthread_mutex_unlock(&tpool->lock);
	pthread_cond_signal(&tpool->cond);

	while (queue.size > 0 || tpool->at_work != 0)
	{
		printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@queue_size: %d@@@@@@@@@@@@@@@@@@@@@@@\n", queue.size);
		printf("front: %d\n", queue.front);
		printf("tail: %d\n", queue.tail);
		printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@list_size: %d@@@@@@@@@@@@@@@@@@@@@@@\n", webg.all_url_list_size);
		printf("$$$$$$$$$$$$$$$$$edge_set_size:  %d\n", webg.edge_set_size);
		printf("at_work: %d\n", tpool->at_work);
		printf("---------------existed_page_size: %d\n", webg.existed_page_size);
		sleep(2);
	}
	printf("finish work!\n");
	tpool_destroy();
	printf("size: %d\n", webg.all_url_list_size);
	printf("queue_size: %d\n", queue.size);
	print_webg_to_file(&webg);
	output_result_file(argc, argv);
	destroy_webg(&webg);
}
