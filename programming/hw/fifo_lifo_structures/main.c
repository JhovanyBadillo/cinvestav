#include <stdio.h>
#include "stack.h"
#include "queue.h"

int main() {
  /* Stack */
  // stack_t *stack = create();
  // push(stack, 3);
  // push(stack, 2);
  // push(stack, 1);
  //
  // printf("pop: %d\n", pop(stack)); // prints 1
  // printf("pop: %d\n", pop(stack)); // prints 2
  // printf("pop: %d\n", pop(stack)); // prints 3
  // printf("pop: %d\n", pop(stack)); // prints -1
  // printf("stack->len: %d\n", stack->len); // prints 0
  // printf("stack->last: %p\n", stack->last); // prints nil
  // delete(stack);
  // stack = nullptr;

  /* Queue */

  queue_t *queue = make_null_queue();
  enqueue(queue, 1);
  enqueue(queue, 2);
  enqueue(queue, 3);
  printf("front: %d\n", front(queue)); // prints 1
  dequeue(queue);
  printf("front: %d\n", front(queue)); // prints 2
  dequeue(queue);
  printf("front: %d\n", front(queue)); // prints 3
  dequeue(queue);

  delete_queue(queue);
  queue = nullptr;

  return 0;
}
