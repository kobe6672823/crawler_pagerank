/*****
** urlqueue.c
** - implements the methods declared in urlqueue.h
*/

#include <stdlib.h>
#include "urlqueue.h"

void queue_init(urlq_t *q)
{
}

int queue_size(urlq_t *q)
{
    return q->size;
}

void queue_push(urlq_t *q, void *element)
{
    q->size++;
}

void *queue_front(urlq_t *q)
{
}

void queue_pop(urlq_t *q)
{
}

void queue_clear(urlq_t *q)
{
}
