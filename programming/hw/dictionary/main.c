#include <stdio.h>

#include "element.h"
#include "dictionary.h"

int main()
{
	dictionary_t *dict = make_null_dictionary(10);

	insert(dict, "a", 1);
	// insert(dict, "a", 2);
	insert(dict, "b", 2);
	insert(dict, "c", 3);
	insert(dict, "a", 2);

	// delete(dict, "a", 1);
	// delete(dict, "b", 2);
	// delete(dict, "c", 3);
	// insert(dict, "d", 4);
	// insert(dict, "e", 5);
	// insert(dict, "f", 6);
	// insert(dict, "g", 7);
	// insert(dict, "h", 8);
	// insert(dict, "i", 9);
	// insert(dict, "j", 10);
	// insert(dict, "k", 11);
	// insert(dict, "l", 12);
	// insert(dict, "l", 12);

	destroy_dictionary(dict);

	return 0;
}