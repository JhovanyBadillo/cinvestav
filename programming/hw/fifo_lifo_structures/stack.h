#ifndef FIFO_LIFO_STRUCTURES_STACK_H
#define FIFO_LIFO_STRUCTURES_STACK_H

#include "node.h"

typedef struct stack stack_t;

struct stack {
  node_t *last;
  int len;
};

stack_t *create();
void delete(stack_t *stack);
int pop(stack_t *stack);
void push(stack_t *stack, int value);

#endif //FIFO_LIFO_STRUCTURES_STACK_H
