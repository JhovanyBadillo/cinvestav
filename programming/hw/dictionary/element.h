#ifndef DICTIONARY_ELEMENT_H
#define DICTIONARY_ELEMENT_H

typedef char* key_t;
typedef int value_t;
typedef struct element element_t;

struct element {
	key_t key;
	value_t value;
	element_t *next;
};

element_t *make_null_element();
element_t *create_element(key_t key, value_t value);
void delete_element(element_t *element);
void print_element(element_t *element);

#endif //DICTIONARY_ELEMENT_H