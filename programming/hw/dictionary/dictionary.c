#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

int member(dictionary_t *dict, class_t key, value_t value)
{
	/* Find the bucket where key lives and then traverse the
	corresponding list if any. 1 if key and value exist,
	-1 otherwise. */

	if (dict == nullptr)
	{
		printf("no valid dictionary\n");
		exit(1);
	}

	const int i = hash_function(key, dict->capacity);

	element_t *current = (dict->hash_table)[i];

	while (current != nullptr)
	{
		if ((current->value) == value && strcmp(key, current->key) == 0)
		{
			return 1;
		}
		current = current->next;
	}

	return -1;
}

void insert(dictionary_t *dict, class_t key, value_t value)
{
	if (dict == nullptr)
	{
		printf("no valid dictionary\n");
		exit(1);
	}

	const int i = hash_function(key, dict->capacity);

	element_t *element = create_element(key, value);

	element_t *current = (dict->hash_table)[i];

	if (current == nullptr)
	{
		/* The ith bucket has no elements */
		(dict->hash_table)[i] = element;
	}
	else
	{
		/* The ith bucket has some elements. If not member, insert
		element at the end. */
		if (member(dict, key, value) < 0)
		{
			while ((current->next) != nullptr)
			{
				current = current->next;
			}

			current->next = element;
		}
		else
		{
			printf("{%s: %d} is already a member (bucket %d)\n", key, value, i);
		}
	}
}

void delete(dictionary_t *dict, class_t key, value_t value)
{
	/* If exists, deletes element with that value. */
	if (dict == nullptr)
	{
		printf("no valid dictionary\n");
		exit(1);
	}

	const int i = hash_function(key, dict->capacity);

	if ((dict->hash_table)[i] != nullptr)
	{
		if (strcmp(((dict->hash_table)[i])->key, key) == 0 &&
			((dict->hash_table)[i])->value == value)
		{
			/* the sought element is the first */
			element_t *next = ((dict->hash_table)[i])->next;
			free((dict->hash_table)[i]->key);
			free((dict->hash_table)[i]);
			(dict->hash_table)[i] = next;
		}
		else
		{
			element_t *current = (dict->hash_table)[i];

			while (current->next != nullptr)
			{
				if (strcmp(current->next->key, key) == 0 && current->next->value == value)
				{
					element_t *element = current->next;
					free(current->next->key);
					free(current->next);
					current->next = element->next;
				}
				else
				{
					current = current->next;
				}
			}
		}
	}
}

void destroy_dictionary(dictionary_t *dict)
{
	if (dict == nullptr)
	{
		return;
	}

	if (dict->hash_table != nullptr)
	{
		int i;
		for (i = 0; i < dict->capacity; i++)
		{
			if (dict->hash_table[i] != nullptr)
			{
				free(dict->hash_table[i]->key);
			}
			free(dict->hash_table[i]);
			/* remains freeing each intermediate element inserted in
			buckets that have not been deleted by operation delete */
		}
		free(dict->hash_table);
	}

	free(dict);
}