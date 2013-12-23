#ifndef _CRAWLER_H_
#define _CRAWLER_H_

#include "webgraph.h"
#include "urlqueue.h"

#define URL_LIST_LEN 5000//url表的长度，默认为1024，即可以放1024条url记录 
#define SITE_IP "127.0.0.1"
#define SITE_PORT 80

void* do_crawler(void *arg); //完成爬取线程的核心流程
int remove_duplicate(int *out_link, int size);

int comp(const void *a,const void *b);

#endif

