#ifndef _URL_QUEUE_H
#define _URL_QUEUE_H

typedef struct _queue {
    int size;
} urlq_t;

/*****
** initialize an empty urlq_t
** must be called first after a new urlq_t is declared
*/ void queue_init(urlq_t *q);


/*****
** push a new element to the end of the urlq_t
** it's up to the client code to allocate and maintain memory of "element"
*/ void queue_push(urlq_t *q, char *element);

/*****
** return the first element in the urlq_t, or NULL when the urlq_t is empty
*/ void *queue_front(urlq_t *q);

/*****
** remove the first element (pointer) from the urlq_t
** set "release" to non-zero if memory deallocation is desired
*/ void queue_pop(urlq_t *q);

/*****
** remove all elements (pointers) from the urlq_t
** set "release" to non-zero if memory deallocation is desired
*/ void queue_clear(urlq_t *q);

/*****
** return current number of elements in the urlq_t, or 0 when urlq_t is empty
*/ int queue_size(urlq_t *q);

#endif /* _RZ_C_QUEUE_ */
