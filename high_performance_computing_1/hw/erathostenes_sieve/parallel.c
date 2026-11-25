#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define n 60

size_t max(size_t a, size_t b)
{
    size_t m;

    if (a <= b)
    {
        m = b;
    }
    else
    {
        m = a;
    }

    return m;
}

int main(int argc, char **argv)
{
    int N = (argc > 1) ? atoi(argv[1]) : n;

    if (N < 0)
    {
        fprintf(stderr, "N should be nonnegative");
        return EXIT_FAILURE;
    }

    double t0 = omp_get_wtime();

    size_t i, j, k;

    size_t bound = (size_t)sqrt(N);
    /* in base we'll store the primes from 2 to sqrt(N).
    We don't know in advance the number of primes, but we guess
    they're at most sqrt(N). */
    char *tmp = (char *)malloc((bound + 1) * sizeof(char));
    /* 0 and 1 are not primes */
    tmp[0] = 0;
    tmp[1] = 0;

    /* initially we assume that every i such that 2 <= i <= sqrt(n)
    is prime */
    for (i = 2; i <= bound; i++)
    {
        tmp[i] = 1;
    }

    size_t count = 0;

    /* sieve sequentially from 2 to sqrt(N) */
    for (i = 2; i <= bound; i++)
    {
        if (tmp[i] != 0)
        {
            for (j = i; i * j <= bound; j++)
            {
                tmp[i * j] = 0;
            }
        }
    }

    /* we count the primes lower than sqrt(N) */
    for (i = 2; i <= bound; i++)
    {
        if (tmp[i] == 1)
        {
            count++;
        }
    }

    /* we put those primes in base */
    size_t *base = malloc(count * sizeof(size_t));

    j = 0;
    for (i = 2; i <= bound; i++)
    {
        if (tmp[i] == 1)
        {
            base[j] = i;
            j++;
        }
    }

    free(tmp);

    /* for each prime in base we sieve now from bound + 1 to N.
    Here one of the tricks is to identify an appropiate multiple of each prime.
    Another trick is to set an appropiate value for chunk. Initially I set it
    to 10, and for N = 100,000,000 and 4 threads it took ~ 30 s.
    Changing chunk to 32,768 (the size of my laptop's L1 cache), it took
    ~ 0.23 s. */
    size_t chunk = 10UL;
    // char *B = (char *)malloc(chunk * sizeof(char));
    size_t upper;
    size_t base_count = count;
    size_t m, prime;

#pragma omp parallel for private(upper, j, prime, m, k) schedule(dynamic) reduction(+ : count)
    for (i = bound + 1; i <= N; i += chunk)
    {
        char B[chunk];

        upper = i + chunk - 1;
        if (upper > N)
        {
            upper = N;
        }

        for (j = 0; j < upper - i + 1; j++)
        {
            /* initially all nonnegative integers (indexes) of B are
            primes. When upper = N, B will have only the first N - i + 1
            positions equal to 1. */

            B[j] = 1;
        }

        for (j = 0; j < base_count; j++)
        {
            prime = base[j];
            m = max(prime * prime, prime * (((i - 1) / prime) + 1));

            for (k = m; k <= upper; k += prime)
            {
                /* the integer k - i is not prime. k - i
                seems strange, but it makes sense because there is
                a displacement from the indexes of B (0 to chunk) to
                the current integers (i to upper) */
                B[k - i] = 0;
            }
        }

        /* we count the primes in block B */
        for (j = 0; i + j <= upper; j++)
        {
            if (B[j] == 1)
            {
                count++;
                // printf("%zu\n", i + j);
            }
        }
    }

    double t1 = omp_get_wtime();

    printf("count: %zu\n", count);

    free(base);

    printf("execution time: %lf\n", t1 - t0);

    return EXIT_SUCCESS;
}