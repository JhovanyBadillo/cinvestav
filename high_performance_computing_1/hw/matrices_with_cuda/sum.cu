#include <stdio.h>

#define M 10240
#define N 10240

double *b, *c;

__global__
void kernelSumaMatrices(double *b, double *c, int m, int n)
{
   int i = threadIdx.x + blockDim.x * blockIdx.x;
   int j = threadIdx.y + blockDim.y * blockIdx.y;

   b[i * n + j] += c[i * n + j];
}

// b = b + c
void sumaMatricesEnDevice(double *b, double *c, int m, int n)
{
   double *bD, *cD;
   int size = m * n * sizeof(double);
   dim3 bloques(320, 320);
   dim3 hilos(32, 32);

   // 1. Asignar memoria
   cudaMalloc(&bD, size);
   cudaMalloc(&cD, size);

   // 2. Copiar datos del Host al Device
   cudaMemcpy(bD, b, size, cudaMemcpyHostToDevice);
   cudaMemcpy(cD, c, size, cudaMemcpyHostToDevice);

   // 3. Ejecutar kernel
   kernelSumaMatrices<<<bloques, hilos>>>(bD, cD, m, n);

   // 4. Copiar datos del device al Host
   cudaMemcpy(b, bD, size, cudaMemcpyDeviceToHost);

   // 5. Liberar Memoria
   cudaFree(bD);
   cudaFree(cD);
}

int main(int argc, char **argv)
{
   int i, j;

   b = (double *)malloc(M * N * sizeof(double));
   c = (double *)malloc(M * N * sizeof(double));

   for (i = 0; i < M; i++)
   {
      for (j = 0; j < N; j++)
      {
         b[i * N + j] = c[i * N + j] = i + j;
      }
   }

   sumaMatricesEnDevice(b, c, M, N);

   for (i = M - 1; i < M; i++)
   {
      for (j = N - 100; j < N; j++)
      {
         printf("%3.2f ", b[i * N + j]);
      }
      printf("\n");
   }

   free(b);
   free(c);
}
