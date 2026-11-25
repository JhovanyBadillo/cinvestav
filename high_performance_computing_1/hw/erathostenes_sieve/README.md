# Criba de Eratóstenes paralelizada

_Miércoles 11 de noviembre, 2026_

<hr/>

## Problema

Desarrollar un programa con OpenMP que, dado un entero $N \geq 2$, determine el número de primos $\leq N$ usando la criba de Eratóstenes.

### Consideraciones

- En un arreglo $A$ de tamaño $N + 1$, un entero no negativo se representa con el índice $i$, $0 \leq i \leq N$, de $A$.
- $A[i] = 1$ sólo si $i$ es primo. En otro caso $A[i] = 0$, es decir, se descarta $i$.
- Los algoritmos consideran que $A$ es inicializado con "unos", es decir, que todos los enteros $i$ son primos y luego descartan los que no.

### Versión secuencial

La criba de Eratóstenes consiste en descartar enteros múltiplos de un conjunto "pequeño" de primos. Comenzando en $2$, se descarta el $4$, el $6$, el $8$, y así hasta el último par $\leq N$. Similarmente para $3, 5, 7,$ hasta el último primo $\leq \lfloor \sqrt N \rfloor$. El siguiente pseudocódigo muestra el algoritmo de cribado secuencial.

```text
CribaSecuencial(N)

▹ #pragma omp parallel for private(j) primer intento para paralelizar
1 for (i ← 2; i ≤ sqrt(N); i++)
2   if (A[i] ≠ 0) ▹ si A[i] = 0 entonces i no es primo y tampoco sus múltiplos
3     for (j ← i; i * j ≤ N; j ← j + 1)
4       A[i * j] ← 0
```

Caí en la trampa de creer que la solución secuencial podría paralelizarse simplemente agregando un "pragma omp" en el primer bucle for. Después, al medir tiempos de ejecución, noté que el tiempo no se reducía según incrementaba el número de hilos. Esto pasa porque los hilos no trabajan sobre datos independientes; dos hilos pueden acceder de forma simultánea a una misma región de memoria. Además, por omisión, la carga se distribuye equitativamente ignorando el hecho de que para $i$ pequeños la carga de trabajo es mayor que para $i$ grandes (los números primos no están distribuidos uniformemente).

### Versión paralelizada

La estrategia es cribar secuencialmente los primeros $\lfloor \sqrt N \rfloor$ enteros y usar los primos resultantes para cribar el resto de enteros $i$, $i \leq \lfloor \sqrt N \rfloor + 1 \leq N,$ por bloques $B$ de `chunk` enteros. Cada uno de estos bloques pueden ser procesados por hilos de forma independiente.

```text
CribaParalelizada(N)

1 base ← array de primos ≤ ⌊sqrt(N)⌋
2 base_count ← base.length
3 count ← base_count
4 #pragma omp parallel for private(upper, j, prime, m, k) schedule(dynamic) reduction(+ : count)
5   for (i ← bound + 1; i ≤ N; i ← i + chunk)
6     char B[chunk]
7     upper ← i + chunk - 1
8     if (upper > N)
9       upper ← N
10    for (j ← 0; j < base_count; j ← j + 1)
11      prime ← base[j]
12      m ← max(prime * prime, prime * ⌈i/prime⌉)
13      for (k ← m; k ≤ upper; k ← k + prime)
14        B[k - i] ← 0
15    for (j ← 0; i + j ≤ upper; j ← j + 1)
16      if (B[j] = 1)
17        count++
```

En la línea 12 se busca el primer múltiplo $m$ de $prime$ tal que $m\cdot prime \geq i$, donde $i$ es el primer entero del bloque $B$ en curso. Si $p \cdot p > i$, ya la hicimos, $m = p$. Si $p \cdot p \leq i$, entonces $m \geq \frac{i} {prime}$, es decir, $m = \lceil \frac{i}{prime} \rceil = \lfloor \frac{i - 1}{prime} \rfloor + 1$, donde $\lfloor \frac{i - 1}{prime} \rfloor$ es la parte entera de $\frac{i - 1}{prime}$.

En la línea 14, el índice $k - i$ considera el desplazamiento que introduce $i$ en los enteros por cribar del bloque en curso. El bucle for de la línea 15 hace el recuento de los números primos encontrados en el bloque en curso. El pragma de la línea 4 agrega las instrucciones para paralelizar el bucle for de la línea 6. Las variables `upper, j, prime, m` y `k` deben ser privadas para cada hilo. Como los tamaños de las cargas de trabajo no son uniformes, se establece `schedule(dynamic)`. Cada hilo hará el recuento de primos en una variable privada `count`. Todas estas, al finalizar la ejecución paralela, se suman.

<br>

**Ejemplo de cribado de un bloque**

- $N = 60$.
- Primos $\leq \lfloor \sqrt N \rfloor$: $2, 3, 5, 7$.
- `chunk = 10`.

Enteros por cribar:

```
 2   3   2      2,3      2   3   2
 |   |   |       |       |   |   |  
 v   v   v       v       v   v   v  
 8,  9, 10, 11, 12, 13, 14, 15, 16, 17  B0
18, 19, 20, 21, 22, 23, 24, 25, 26, 27  B1
28, 29, 30, 31, 32, 33, 34, 35, 36, 37  B2
38, 39, 40, 41, 42, 43, 44, 45, 46, 47  B3
48, 49, 50, 51, 52, 53, 54, 55, 56, 57  B4
58, 59, 60                              B5
```

Cuando se procesa el bloque `B0`, los enteros $8, 10, 12, 14$ y $16$ se descartan con el $2$. Los enteros $9$ y $15$ se descartan con el $3$. Los primos $5$ y $7$ no descartan en este bloque. Obsérvese que el $12$ se descarta con el $2$ y con el $3$. La operación `descartar` es idempotente. El recuento de primos en este bloque es $3$.


### Observaciones

- El tamaño de `chunk` es decisivo para el rendimiento del programa.

- Para $N = 10^8$ y $4$ hilos de ejecución, con `chunk = 10` el tiempo de ejecución del programa (fracción secuencial y fracción paralelizable) fue de ~$30$ s. Con `chunk = 32,768`, o sea $32$ KiB, el tamaño de la caché L1 de un núcleo de la plataforma de ejecución, el tiempo se desplomó a ~$0.23$ s.