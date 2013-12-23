#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include <pthread.h>

#include "urlqueue.h"
#include "webgraph.h"

#define INIT_POOL_FAIL -1
#define INIT_POOL_SUCCEED 0

typedef struct tpool
{
	int shutdown;
	int max_thr_num;
	int at_work;
	pthread_t *thr_id;
	pthread_mutex_t lock;
	pthread_cond_t cond;
}tpool_t;

struct para_for_crawler
{
	struct web_graph *webg;
	urlq_t *queue;
	tpool_t *tpool;
};

tpool_t* tpool_create(int max_thr_num, struct web_graph *webg, urlq_t *queue);

void tpool_destroy();

#endif
