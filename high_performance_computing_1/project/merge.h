#ifndef MERGE_H
#define MERGE_H

#include <stdlib.h>

#include "types.h"

void merge(data_t *arr, size_t inf, size_t mid, size_t sup)
{
    size_t i, j, k;
    size_t n1 = mid - inf + 1;
    size_t n2 = sup - mid;

    data_t *L = (data_t *)malloc(n1 * sizeof(data_t));
    data_t *R = (data_t *)malloc(n2 * sizeof(data_t));

    if (L == NULL || R == NULL)
    {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < n1; i++)
        L[i] = arr[inf + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    i = 0;
    j = 0;
    k = inf;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

void merge_partitions(data_t *arr, limits_t *limits, size_t num_partitions)
{
    size_t step;
    for (step = 1; step < num_partitions; step *= 2)
    {
        size_t i;
        for (i = 0; i + step < num_partitions; i += 2 * step)
        {
            size_t inf = limits[i].inf;
            size_t mid = limits[i + step - 1].sup;
            size_t sup = limits[i + 2 * step - 1 < num_partitions ? i + 2 * step - 1 : num_partitions - 1].sup;

            merge(arr, inf, mid, sup);
        }
    }
}

#endif // MERGE_H