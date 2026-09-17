#ifndef FIFO_LIFO_STRUCTURES_NODE_H
#define FIFO_LIFO_STRUCTURES_NODE_H

typedef struct node node_t;

struct node {
  int value;
  node_t *next;
  node_t *previous;
};

node_t *create_node(int value);
void delete_node(node_t *node);

#endif //FIFO_LIFO_STRUCTURES_NODE_H
