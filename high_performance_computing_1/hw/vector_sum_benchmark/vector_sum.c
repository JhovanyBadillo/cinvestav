#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <omp.h>

#define N 100000000

double *u, *v, *w;

int main(int argc, char **argv)
{
    size_t n, i;

    n = (argc > 1) ? atoi(argv[1]) : 1;

    u = (double *)malloc(N * sizeof(double));
    v = (double *)malloc(N * sizeof(double));
    w = (double *)malloc(N * sizeof(double));

    for (i = 0; i < N; i++)
    {
        double di = (double)i;
        u[i] = 2.0 * di;
        v[i] = M_PI * di;
    }

#pragma omp parallel num_threads(n)
    {
    }

    double t0 = omp_get_wtime();

#pragma omp parallel for num_threads(n) schedule(static)
    for (i = 0; i < N; i++)
    {
        w[i] = u[i] + v[i];
    }

    double t1 = omp_get_wtime();

    printf("%lf\n", t1 - t0);

    free(u);
    free(v);
    free(w);
}