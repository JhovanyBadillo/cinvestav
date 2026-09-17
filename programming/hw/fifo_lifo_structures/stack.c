#include <stdlib.h>
#include "stack.h"

stack_t *create() {
  stack_t *stack = malloc(sizeof *stack);

  stack->last = nullptr;
  stack->len = 0;

  return stack;
}

void delete(stack_t *stack) {
  if (stack == nullptr) {
    return;
  }

  if (stack->last == nullptr) {
    free(stack);

    return;
  }

  while (pop(stack) != -1) {
  }

  free(stack);
}

void push(stack_t *stack, const int value) {
  /* Shortcoming: stack_t stacks cannot store -1 */
  if (stack == nullptr) {
    return;
  }

  if (value == -1) {
    exit(1);
  }

  node_t *node = create_node(value);

  if (stack->last == nullptr) {
    stack->last = node;
  } else {
    node_t *previous_last = stack->last;
    stack->last = node;
    node->next = previous_last;
  }

  stack->len += 1;
}

int pop(stack_t *stack) {
  if (stack == nullptr || stack->last == nullptr) {
    return -1;
  }

  node_t *old_last = stack->last;
  const int out = old_last->value;

  stack->last = old_last->next;

  delete_node(old_last);

  stack->len -= 1;

  return out;
}
