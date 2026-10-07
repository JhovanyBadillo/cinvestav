#include <stdio.h>
#include "element.h"
#include "dictionary.h"

extern void print_dictionary(dictionary_t *dict);

int main()
{
	dictionary_t *dict = make_null_dictionary(10);

	insert(dict, "Ana", 20);
	insert(dict, "Ana", 30);
	insert(dict, "Rafa", 20);
	insert(dict, "Carlos", 20);

	printf("\ndictionary:\n");
	print_dictionary(dict);

	delete(dict, "Ana", 20);

	printf("\ndictionary:\n");
	print_dictionary(dict);

	destroy_dictionary(dict);

	return 0;
}
