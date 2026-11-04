/*
- implementar un programa que
    - inicialice 2 matrices cuadradas de tamaño N
    - sume las dos matrices y coloque el resultado en una matriz C
    - los elementos de las matrices deben ser de tipo double
- ejecutar con un hilo el programa anterior y medir rendimiento (T(N, 1))
- ejecutar con p hilos y medir rendimiento (T(N, p))
- graficar aceleración Psi(N, p) = T(N, 1) / T(N, p)
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

double *A, *B, *C;

int main(int argc, char **argv)
{
    u_int64_t N = 20000;

    A = (double *)malloc(N * N * sizeof(double));
    B = (double *)malloc(N * N * sizeof(double));
    C = (double *)malloc(N * N * sizeof(double));

    size_t i, j, n;

    n = (argc > 1) ? atoi(argv[1]) : 1;

    double t0, t1, t2;

    t0 = omp_get_wtime();

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            A[i * N + j] = (double)(i + j);
            B[i * N + j] = M_PI * (double)(i + j);
            C[i * N + j] = 0.0;
        }
    }

    t1 = omp_get_wtime();

#pragma omp parallel for num_threads(n) private(j)
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            C[i * N + j] = A[i * N + j] + B[i * N + j];
        }
    }

    t2 = omp_get_wtime();

    printf("tiempo secuencial: %lf\n", t1 - t0);
    printf("tiempo paralelo: %lf\n", t2 - t1);
    printf("tiempo total: %lf\n", t2 - t0);

    return 0;
}
