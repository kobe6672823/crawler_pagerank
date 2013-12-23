#ifndef _URL_QUEUE_H
#define _URL_QUEUE_H

#define QUEUE_SIZE 300000
#define INIT_QUEUE_FAIL 0
#define INIT_QUEUE_SUCCEED 1

typedef struct _queue {
    int size;
	int *queue;
	int front;
	int tail;
} urlq_t;

/*****
** initialize an empty urlq_t
** must be called first after a new urlq_t is declared
*/ int queue_init(urlq_t *q);


/*****
** push a url_number to the end of the urlq_t
*/ void queue_push(urlq_t *q, int url_num);

/*****
** return the first element in the urlq_t, or -1 when the queue is empty
*/ int queue_front(urlq_t *q);

/*****
** remove the first url_number from the urlq_t, and return it
** set "release" to non-zero if memory deallocation is desired
*/int queue_pop(urlq_t *q);

/*****
** remove all elements (pointers) from the urlq_t
** set "release" to non-zero if memory deallocation is desired
*/ void queue_clear(urlq_t *q);

/*****
** return current number of elements in the urlq_t, or 0 when urlq_t is empty
*/ int queue_size(urlq_t *q);

#endif /* _RZ_C_QUEUE_ */
