#ifndef FIFO_LIFO_STRUCTURES_QUEUE_H
#define FIFO_LIFO_STRUCTURES_QUEUE_H

#include "node.h"

#define NULL_QUEUE "Does not exist queue\n"
#define EMPTY_QUEUE "Empty queue\n"

typedef struct queue queue_t;

struct queue {
  node_t *front;
  node_t *rear;
};

/* Makes queue an empty list */
queue_t *make_null_queue();

/* Returns the first value on queue */
int front(const queue_t *queue);

/* Inserts value at the end of queue  */
void enqueue(queue_t *queue, int value);

/* Deletes the first element of queue */
void dequeue(queue_t *queue);

/* Returns 1 if and only if queue is an empty queue */
int empty_queue(const queue_t *queue);

/* Delete the queue and its nodes, if any */
void delete_queue(queue_t *queue);

#endif //FIFO_LIFO_STRUCTURES_QUEUE_H
