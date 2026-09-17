#include <stdio.h>
#include "stack.h"

int main() {
  stack_t *stack = create();
  push(stack, 3);
  push(stack, 2);
  push(stack, 1);

  printf("pop: %d\n", pop(stack)); // prints 1
  printf("pop: %d\n", pop(stack)); // prints 2
  printf("pop: %d\n", pop(stack)); // prints 3
  printf("pop: %d\n", pop(stack)); // prints -1
  printf("stack->len: %d\n", stack->len); // prints 0
  printf("stack->last: %p\n", stack->last); // prints nil
  delete(stack);
  stack = nullptr;

  return 0;
}
