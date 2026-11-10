#include <stdlib.h>
#include <pthread.h>

#include "types.h"

typedef struct
{
    data_t *data;
    size_t inf;
    size_t sup;
} thread_bundle_t;

int compare_doubles(const void *a, const void *b)
{
    data_t arg1 = *(const data_t *)a;
    data_t arg2 = *(const data_t *)b;

    if (arg1 < arg2)
        return -1;
    if (arg1 > arg2)
        return 1;

    return 0;
}

void *sort(void *thread_bundle)
{
    thread_bundle_t bundle = *(thread_bundle_t *)thread_bundle;

    size_t number_of_elements = bundle.sup - bundle.inf + 1;
    qsort(bundle.data + bundle.inf, number_of_elements, sizeof(data_t), compare_doubles);

    pthread_exit(NULL);
}