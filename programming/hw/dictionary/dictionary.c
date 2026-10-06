#include <stdio.h>
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

	if (dict == nullptr)
	{
		printf("failed malloc\n");
		exit(1);
	}

	dict->capacity = capacity;
	dict->hash_table = malloc(capacity * sizeof(element_t *));

	if ((dict->hash_table) == nullptr)
	{
		printf("failed malloc\n");
		free(dict);
		exit(1);
	}

	int i;
	for (i = 0; i < capacity; i++)
	{
		(dict->hash_table)[i] = nullptr;
	}

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