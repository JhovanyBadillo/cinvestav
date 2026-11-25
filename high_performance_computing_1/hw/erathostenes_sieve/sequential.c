#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define n 60

int main(int argc, char **argv)
{
    int N = (argc > 1) ? atoi(argv[1]) : n;

    if (N < 0)
    {
        fprintf(stderr, "N should be nonnegative");
        return EXIT_FAILURE;
    }

    char *A = (char *)malloc((N + 1) * sizeof(char));
    if (A == NULL)
    {
        fprintf(stderr, "malloc failed");
        return EXIT_FAILURE;
    }

    size_t i, j;
    for (i = 0; i <= N; i++)
    {
        A[i] = 1;
    }

    size_t bound = (size_t)sqrt(N);

    for (i = 2; i <= bound; i++)
    {
        if (A[i] != 0)
        {
            for (j = i; i * j <= N; j++)
            {
                A[i * j] = 0;
            }
        }
    }

    // for (i = 2; i <= bound; i++)
    // {
    //     if (A[i] != 0)
    //     {
    //         for (j = i * i; j <= N; j += i)
    //         {
    //             A[j] = 0;
    //         }
    //     }
    // }

    size_t count = 0;

    /* 0 and 1 are not primes. We start from 2 */
    for (i = 2; i <= N; i++)
    {
        if (A[i] == 1)
        {
            // printf("%zu\n", i);
            count++;
        }
    }

    printf("count: %zu\n", count);

    free(A);

    return EXIT_SUCCESS;
}