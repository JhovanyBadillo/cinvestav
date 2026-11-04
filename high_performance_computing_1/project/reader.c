#include <stdio.h>

#include "divider.h"

#define NUM_CORES 8 /* one process per physical core */
#define SIZE 10000

int main()
{
    limits_t limits[NUM_CORES];

    divider(NUM_CORES, SIZE, limits);

    size_t i;

    for (i = 0; i < NUM_CORES; i++)
    {
        printf("i: %ld\n", i);
        printf("\tinf: %ld sup: %ld\n", limits[i].inf, limits[i].sup);
    }

    return 0;
}