#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "element.h"

element_t *create_element(class_t key, value_t value)
{
	element_t *element = malloc(sizeof *element);

	element->key = strdup(key);
	element->value = value;
	element->next = nullptr;

	return element;
}

void print_element(element_t *element)
{
	printf("key: %s\n", element->key);
	printf("value: %d\n", element->value);
}

void delete_element(element_t *element)
{
	if (element == nullptr)
	{
		return;
	}

	free(element);
}