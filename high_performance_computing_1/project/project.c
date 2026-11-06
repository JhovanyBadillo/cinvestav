/*

proceso main:
establecer limites para cada proceso ./
fork() por cada nucleo fisico ./
a cada proceso hijo le tenemos que dar sus limites del archivo (via pipe) ./

subproceso 1:
crear dos hilos
determinar, a partir de los limites del subproceso, los limites de lectura
de cada hilo, inicio_hiloM, fin_hiloM
    hilo i:
        *fp = fopen(archivo gigante)
        leer de *fp los datos que le tocan (inicio_hilo1, fin_hilo1)
        escribir a un archivo en /tmp/XXX lo que leyó
    espera a los hilos
    lee una linea de tmp_file_1 -> line_1(10 numeros en crudo)
    lee una linea de tmp_file_2 -> line_2 (10 numeros en crudo)
    interprete un numero double de line_1 -> double numero_1
    interprete un numero double de line_2 -> double numero_2
    comparar(numero_1, numero_2) -> escribir resultado en archivo temporal tmp_subproceso1
    comunicar identificador de archivo temporal a proceso main

...

lo mismo para el resto de subprocesos

proceso main:
    tendra 4 identificadores de archivos temporales con numeros ordenados
    mergesort(archivo1, archivo2) -> ordenado_1
    mergesort(archivo3, archivo4) -> ordenado_2
    mergesort(ordenado_1, ordenado_2) -> ordenado_final
    escribir en un archivo no temporal el ordenamiento final


consideraciones:
    fijar en 4 el numero de nucleos
    subprocesos con fork
    el archivo gigante contiene en binario M doubles

por ver:
    determinar dinámicamente el número de núcleos físicos y con esto
    determinar las particiones

*/

#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

#include "divider.h"

#define NUM_CORES 4 /* one process per physical core */
#define SIZE 10

typedef double data_t;

int main()
{
    size_t i;
    pid_t pid;
    limits_t limits[NUM_CORES];

    divider(NUM_CORES, SIZE, limits);

    /* Maps the file in the virtual memory and makes it accessible to
    child processes. */
    const char *filename = "./data/random_matrix_10.bin";
    int file = open(filename, O_RDWR);
    void *mapped_file = mmap(NULL, SIZE * SIZE * sizeof(data_t), PROT_READ | PROT_WRITE, MAP_SHARED, file, 0);
    close(file);

    if (mapped_file == MAP_FAILED)
    {
        printf("mmap failed\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < NUM_CORES; i++)
    {
        pid = fork();

        if (pid == -1)
        {
            printf("fork failed\n");
            _exit(EXIT_FAILURE);
        }

        if (pid == 0)
        {
            limits_t bundle = limits[i];

            size_t i;
            for (i = bundle.inf; i <= bundle.sup; i++)
            {
                printf("[%d] %lf\n", getpid(), ((data_t *)mapped_file)[i]);
            }

            _exit(EXIT_SUCCESS); /* necessary for the for loop to terminate */
        }
    }

    for (i = 0; i < NUM_CORES; i++)
    {
        wait(NULL);
    }

    printf("parent was %d\n", getpid());

    munmap(mapped_file, SIZE * SIZE * sizeof(data_t));

    exit(EXIT_SUCCESS);
}