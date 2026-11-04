/*

Un programa que, dado el tamaño de una matriz cuadrada A y el número de núcleos
(procesos), determina los índices inferior y superior de las filas de A que le
corresponden a cada proceso, considerando un proceso por núcleo.

Para grandes volúmenes de datos: mmap.
es una llamada al sistema de Unix/Linux que permite mapear archivos, dispositivos o
memoria en el espacio de direcciones virtuales de un proceso, permitiendo acceder a
ellos como si fueran matrices en memoria.

*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef struct limits limits_t;

struct limits
{
    size_t inf;
    size_t sup;
};

void divider(size_t num_cores, size_t size, limits_t *limits)
{
    if (num_cores > size)
    {
        printf("There are more cores than lines to process\n");
        exit(1);
    }

    int OFFSET = (int)floor(size / num_cores);

    size_t i;
    limits_t bundle;

    if ((size % num_cores) == 0)
    {
        for (i = 0; i < num_cores; i++)
        {
            bundle.inf = OFFSET * i;
            bundle.sup = OFFSET * (i + 1) - 1;

            limits[i] = bundle;
        }
    }
    else
    {
        for (i = 0; i < num_cores - 1; i++)
        {
            bundle.inf = OFFSET * i;
            bundle.sup = OFFSET * (i + 1) - 1;

            limits[i] = bundle;
        }

        bundle.inf = OFFSET * i;
        bundle.sup = size - 1;

        limits[i] = bundle;
    }
}