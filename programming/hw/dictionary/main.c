#include <stdio.h>
#include "element.h"
#include "dictionary.h"

int main() {
	dictionary_t *dict = make_null_dictionary(10);
	
	printf("hash('abc') = %d", hash_function("abc", 10));
	
	destroy_dictionary(dict);

	return 0;

}