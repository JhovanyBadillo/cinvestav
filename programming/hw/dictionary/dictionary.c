#include <stdlib.h>
#include "dictionary.h"

int hash_function(class_t key, int capacity)
{
	/* hash function suggested by an LLM */
	unsigned int hash = 5381;
	int c;

	while ((c = *key++))
	{
		hash = ((hash << 5) + hash) + c;
	}

	return hash % capacity;
}

dictionary_t *make_null_dictionary(int capacity)
{
	dictionary_t *dict = malloc(sizeof *dict);

	dict->capacity = capacity;
	dict->hash_table = malloc(capacity * sizeof(element_t));

	return dict;
}

void destroy_dictionary(dictionary_t *dict)
{
	if (dict == nullptr)
	{
		return;
	}

	free(dict);
}