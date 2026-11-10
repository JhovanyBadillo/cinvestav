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
#include <pthread.h>

#include "parser.h"
#include "divider.h"
#include "sorting.h"

#define NUM_CORES 4 /* one process per physical core */
#define NUM_THREADS 2

int main()
{
    size_t i;
    pid_t pid;
    limits_t limits[NUM_CORES];

    size_t number_of_elements;
    data_t *mapped_binary_file;

    /* Parses into floats, maps the file in the virtual memory and
    makes it accessible to child processes. */
    parse(
        "./data/matriz_a_one_line.txt",
        "./data/matriz_a_one_line.bin",
        &mapped_binary_file,
        &number_of_elements);

    divider(NUM_CORES, number_of_elements, limits);

    for (i = 0; i < NUM_CORES; i++)
    {
        pid = fork();

        if (pid == -1)
        {
            perror("fork failed");
            _exit(EXIT_FAILURE);
        }

        if (pid == 0)
        {
            limits_t bundle = limits[i];
            thread_bundle_t thread_bundles[NUM_THREADS];
            size_t mid = (bundle.inf + bundle.sup) / 2;

            thread_bundles[0].data = mapped_binary_file;
            thread_bundles[1].data = mapped_binary_file;

            pthread_t threads[NUM_THREADS];
            int thread_result;

            pthread_attr_t attr;
            pthread_attr_init(&attr);
            pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

            size_t j;
            /* thread 1 */
            /* prepares thread bundle: inf and sup thread limits */
            thread_bundles[0].inf = bundle.inf;
            thread_bundles[0].sup = mid;

            thread_result = pthread_create(&threads[0], NULL, sort, &thread_bundles[0]);

            if (thread_result)
            {
                perror("thread creation failed");
                exit(EXIT_FAILURE);
            }

            /* thread 2 */
            /* prepares thread bundle: inf and sup thread limits */
            thread_bundles[1].inf = mid + 1;
            thread_bundles[1].sup = bundle.sup;

            thread_result = pthread_create(&threads[1], NULL, sort, &thread_bundles[1]);

            if (thread_result)
            {
                perror("thread creation failed");
                exit(EXIT_FAILURE);
            }

            pthread_attr_destroy(&attr);

            for (j = 0; j < NUM_THREADS; j++)
            {
                thread_result = pthread_join(threads[j], NULL);
                if (thread_result)
                {
                    perror("failed to join thread");
                    exit(EXIT_FAILURE);
                }
            }

            _exit(EXIT_SUCCESS); /* necessary for the for loop to terminate */
        }
    }

    for (i = 0; i < NUM_CORES; i++)
    {
        wait(NULL);
    }

    for (i = 0; i < 100; i++)
    {
        printf("%f\n", (mapped_binary_file)[i]);
    }

    munmap(mapped_binary_file, number_of_elements * sizeof(data_t));

    exit(EXIT_SUCCESS);
}