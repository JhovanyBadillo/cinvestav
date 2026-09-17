#include <stdlib.h>
#include "node.h"

node_t *create_node(const int value) {
  /*
   * sizeof *node es equivalente a sizeof(node_t).
   * Dado el scope de *node, el compilador ya puede
   * determinar su tipo de dato
   */
  node_t *node = malloc(sizeof *node);

  node->value = value;
  node->next = nullptr;

  return node;
}

void delete_node(node_t *node) {
  if (node == nullptr) {
    return;
  }

  free(node);
}
