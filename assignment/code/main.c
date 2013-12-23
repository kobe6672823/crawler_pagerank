/*****
** main.c
** - 主线程，维持线程池，线程间通信等
*/
#include <stdio.h>
#include <pthread.h>

#include "crawler.h"
#include "http_client.h"
#include "urlqueue.h"
#include "webgraph.h"
#include "link_parser.h"

#define MAX_THREADS     1024

typedef struct _threadpool_st {
    pthread_t threadinfo;	//pointer to threads
    pthread_mutex_t waitlock; //等待URL队列任务
    int idle; //当url队列为空时置位为1，否则为0
}threadpool_item;

threadpool_item tpool[MAX_THREADS];
int num_threads;	//number of threads

int main()
{
    
}


