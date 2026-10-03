#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "element.h"

typedef struct dictionary dictionary_t;

struct dictionary {
	int capacity;
	element_t **hash_table;
};

int hash_function(key_t key, int capacity);
dictionary_t *make_null_dictionary(int capacity);
int member(dictionary_t *dict, key_t key, value_t value);
void insert(dictionary_t *dict, key_t key, value_t value);
void delete(dictionary_t *dict, key_t key, value_t value);
void destroy_dictionary(dictionary_t *dict);

#endif //DICTIONARY_H