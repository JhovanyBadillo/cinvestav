#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

/* This queue implementation uses a sentinel node.
 * A queue begins with a single node, the sentinel node. Both
 * front and rear point to this node and in this case we say
 * the queue is empty. Thus, at every time the queue has n + 1
 * nodes. This is based on the queue operations discussed in
 * the book "Data Structures and Algorithms" by  Aho, Hopcroft
 * and Ullman.
 */

queue_t *make_null_queue()
{
  queue_t *queue = malloc(sizeof *queue);
  queue->front = make_null_node(); // creates the sentinel node
  queue->front->next = nullptr;
  queue->rear = queue->front;

  return queue;
}

int front(const queue_t *queue)
{
  if (queue == nullptr)
  {
    printf(NULL_QUEUE);
    exit(1);
  }

  if (empty_queue(queue))
  {
    printf(EMPTY_QUEUE);
    exit(1);
  }

  return queue->front->next->value;
}

void enqueue(queue_t *queue, const int value)
{
  if (queue == nullptr)
  {
    printf(NULL_QUEUE);
    exit(1);
  }

  queue->rear->next = create_node(value);

  queue->rear = queue->rear->next;
  queue->rear->next = nullptr;
}

void dequeue(queue_t *queue)
{
  if (queue == nullptr)
  {
    printf(NULL_QUEUE);
    exit(1);
  }

  if (empty_queue(queue))
  {
    printf(EMPTY_QUEUE);
    exit(1);
  }

  node_t *current_front = queue->front;

  queue->front = queue->front->next;

  delete_node(current_front);
}

int empty_queue(const queue_t *queue)
{
  if (queue == nullptr)
  {
    printf(NULL_QUEUE);
    exit(1);
  }

  return queue->front == queue->rear;
}

void delete_queue(queue_t *queue)
{
  if (queue == nullptr)
  {
    return;
  }

  while (!empty_queue(queue))
  {
    dequeue(queue);
  }

  delete_node(queue->front); // deletes the sentinel node
  free(queue);
}
