/*****
** urlqueue.c
** - implements the methods declared in urlqueue.h
*/

#include <stdlib.h>
#include "urlqueue.h"
#include <string.h>
#include <stdio.h>

//这个queue约占600KB
int queue_init(urlq_t *q)
{
	int i, j;

	q->size = 0;
	q->queue = (int *)malloc(sizeof (int ) * QUEUE_SIZE);
	if (q->queue == NULL)
	{
		printf("为队列分配内存失败！\n");
		return INIT_QUEUE_FAIL;
	}
	memset(q->queue, 0, sizeof (int ) * QUEUE_SIZE);
	q->front = q->tail = 0;
	return INIT_QUEUE_SUCCEED;
}

int queue_size(urlq_t *q)
{
    return q->size;
}

void queue_push(urlq_t *q, int url_num)
{
    q->size++;
	q->queue[q->tail] = url_num;
	q->tail = (q->tail + 1) % QUEUE_SIZE;
}

int queue_front(urlq_t *q)
{
	return q->queue[q->front];
}

int queue_pop(urlq_t *q)
{
	int f = q->queue[q->front];

	if (q->size == 0)
		return -1;

	q->front = (q->front + 1) % QUEUE_SIZE;
	q->size--;
	return f;
}

void queue_clear(urlq_t *q)
{
	free(q->queue);
}
