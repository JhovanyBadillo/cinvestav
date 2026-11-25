# Operaciones matriciales con CUDA

_Miércoles 25 de noviembre, 2026_

<hr/>

## Problemas

**1** Escribir un programa en CUDA que calcule la suma de dos matrices $B, C \in \mathbb{R}^{M\times N}$ usando memoria solamente para $B$ y $C$ (a diferencia del ejemplo visto en clase, este programa no debe usar una matriz $A \in \mathbb{R}^{M\times N}$ para guardar la suma). Considerando que $M = N$, calcular el tamaño máximo que pueden tener $B$ y $C$. La memoria de video es de $5040$ MiB.

**2** Escribir un programa en CUDA que calcule la transpuesta $A^{T}$ de una matriz $A \in \mathbb{R}^{M\times N}$.

## Consideraciones

1. En CUDA C se usa la disposición _row-major_ para "linealizar" un arreglo 2D. Para acceder a la entrada $(A)_{ij}$ de una matriz en la variable `A`, se usa `A[i * N + j]`, "`i * N`" avanza $i$ filas y "`+ j`" avanza hasta el elemento $j$ de la fila $i$. $A \in \mathbb{R}^{M\times N}$.

2. Para el problema 1 se reutiliza gran parte del programa visto en clase `sumaMatrices1.cu`.

3. $1$ MiB $=$ $2^{20}$ bytes. $1$ `double` $= 2^3$ bytes.

## Soluciones

### 1

Dada la consideración 2, en lugar de hacer la asignación de la suma de $B$ y $C$ como `a = b + c`, se hace `b = b + c`. Entonces se sobreescribe `b` con el resultado de la suma. El programa completo está en [sum.cu](./sum.cu).

```C
__global__
void kernelSumaMatrices(double *b, double *c, int m, int n)
{
   int i = threadIdx.x + blockDim.x * blockIdx.x;
   int j = threadIdx.y + blockDim.y * blockIdx.y;

   b[i * n + j] += c[i * n + j];
}
```

Al tener $S = 5040$ MiB de memoria disponibles, ¿cuál es el tamaño máximo $T$ de una matriz cuadrada con elementos de doble precisión tal que pueda almacenarse dos veces?

$$
S \geq 2\cdot T^2 \cdot 2^3
$$

$$
T \leq \sqrt{\frac{5040\cdot 2^{20}}{2\cdot 2^3}}
$$

$$
T = 18,174
$$

Un incremento de más del $22\%$ frente a los $14,839$ elementos usando una tercera matriz para almacenar la suma.

### 2

Si $A \in \mathbb{R}^{M\times N}$, entonces $A^{T} \in \mathbb{R}^{N\times M}$. En la transposición de la matriz $A$ se "invierten los papeles" de los índices, así como de sus rangos. $(A)_{ij}$ es la entrada en la $i$-ésima fila y $j$-ésima columna de $A$, donde $0 \leq i < M$ y $0 \leq j < N$. En la matriz transpuesta $B = A^T$ la entrada correspondiente es $(B)_{ji}$, donde $0 \leq j < M$ y $0 \leq i < N$. Mientras que en $A$ hay $M$ filas y $N$ columnas, en $B$ hay $N$ filas y $M$ columnas.

El siguiente kernel de CUDA ejecuta la operación de transposición de una matriz $A$ y almacena el resultado en una matriz $B$.

```C
__global__
void kernelTranspose(double *a, double *b, int m, int n)
{
    int i = threadIdx.x + blockDim.x * blockIdx.x;
    int j = threadIdx.y + blockDim.y * blockIdx.y;

    b[j * m + i] = a[i * n + j];
}
```

`m` y `n` son $M$ y $N$, respectivamente. Dada la consideración 1, la asignación se interpreta como $(B)_{ji} = (A)_{ij}$, donde $B$ tiene $M$ columnas y $N$ filas.

El programa completo que transpone matrices se encuentra en el archivo [transpose.cu](./transpose.cu). Al ejecutar este programa con $M = 3$ y $N = 4$, sobre una matriz $A$ tal que, para todo $i \in \{0, 1, 2\}$, $j \in \{0, 1, 2, 3\}$, $(A)_{ij} = i$, se obtiene lo siguiente.

```text
A:   0.0, 0.0, 0.0, 0.0
     1.0, 1.0, 1.0, 1.0
     2.0, 2.0, 2.0, 2.0

A^T: 0.0, 1.0, 2.0
     0.0, 1.0, 2.0
     0.0, 1.0, 2.0
     0.0, 1.0, 2.0
```

Para $M, N$ tales que $M\cdot N > 1024$, debe usarse una malla y número de hilos adecuados, por ejemplo una malla cuadrada de $320$ bloques por eje con $32\times 32$ hilos en cada bloque para $M = N = 10240$.