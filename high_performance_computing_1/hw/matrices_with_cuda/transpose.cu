#include <stdio.h>
#include <stdlib.h>

#define M 10240 /* filas */
#define N 10240 /* columnas */

/* sequential transposition */
void transpose(double *A, double *B, int m, int n)
{
    size_t i, j;

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            B[i * m + j] = A[j * n + i];
        }
    }
}

__global__
void kernelTranspose(double *a, double *b, int m, int n)
{
    int i = threadIdx.x + blockDim.x * blockIdx.x;
    int j = threadIdx.y + blockDim.y * blockIdx.y;

    b[j * m + i] = a[i * n + j];
}

void transposeOnDevice(double *a, double *b, int m, int n)
{
    double *aD, *bD;
    
    /* asignar memoria en el device */
    size_t size = m * n * sizeof(double);
    cudaMalloc(&aD, size);
    cudaMalloc(&bD, size);

    /* copiar datos del host al device */
    cudaMemcpy(aD, a, size, cudaMemcpyHostToDevice);

    /* ejecutar el kernel */
    dim3 bloques(320, 320);
    dim3 hilos(32, 32);

    kernelTranspose<<<bloques, hilos>>>(aD, bD, m, n);

    /* copiar datos del device al host */
    cudaMemcpy(b, bD, size, cudaMemcpyDeviceToHost);

    /* liberar memoria en el device */
    cudaFree(aD);
    cudaFree(bD);
}

int main()
{
    double *A, *B;

    A = (double *)malloc(M * N * sizeof(double));
    B = (double *)malloc(M * N * sizeof(double));

    /* poblar entradas de A */
    size_t i, j;
    for (i = 0; i < M; i++)
    {
        for (j = 0; j < N; j++)
        {
            A[i * N + j] = i;
        }
    }

    /* ver la primera fila de A */
    // for (i = 0; i < M; i++)
    // {
        for (j = 0; j < N; j++)
        {
            printf("%2.1lf ", A[0 * N + j]);
        }
        printf("\n");
    // }

    /* sequential transposition */
    // transpose(A, B, M, N);

    /* parallel transposition */
    transposeOnDevice(A, B, M, N);

    printf("A^T\n");

    /* ver la primera fila de B = A^T */
    // for (i = 0; i < N; i++)
    // {
        for (j = 0; j < M; j++)
        {
            printf("%2.1lf ", B[0 * M + j]);
        }
        printf("\n");
    // }

    free(A);
    free(B);
}
