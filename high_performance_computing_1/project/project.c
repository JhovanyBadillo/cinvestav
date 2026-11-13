/*

*/

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <pthread.h>

#include "parser.h"
#include "divider.h"
#include "sorting.h"
#include "merge.h"

#define NUM_THREADS 2

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s <file> <cores> <dump>\n", argv[0]);
        fprintf(stderr, "Arguments:\n");
        fprintf(stderr, "  file: Path to the input data file\n");
        fprintf(stderr, "  cores: Number of physical cores to use\n");
        fprintf(stderr, "  dump: Dump sorted data (y/n)\n");

        exit(EXIT_FAILURE);
    }

    char *data_filename = argv[1];
    size_t cores = (size_t)atoi(argv[2]);
    size_t dump;

    if (strcasecmp("y", argv[3]) == 0)
    {
        dump = 1;
    }
    else if (strcasecmp("n", argv[3]) == 0)
    {
        dump = 0;
    }
    else
    {
        fprintf(stderr, "<dump>: Dump sorted data to text file (y/n)\n");
        exit(EXIT_FAILURE);
    }

    size_t i;
    pid_t pid;
    limits_t limits[cores];

    size_t number_of_elements;
    data_t *mapped_binary_file;

    /* Parses into floats, maps the file in the virtual memory and
    makes it accessible to child processes. */
    parse(
        data_filename,
        "./matrix.bin",
        &mapped_binary_file,
        &number_of_elements);

    divide(cores, number_of_elements, limits);

    for (i = 0; i < cores; i++)
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

            merge(mapped_binary_file, bundle.inf, mid, bundle.sup);

            _exit(EXIT_SUCCESS); /* necessary for the for loop to terminate */
        }
    }

    for (i = 0; i < cores; i++)
    {
        wait(NULL);
    }

    merge_partitions(mapped_binary_file, limits, cores);
    msync(mapped_binary_file, number_of_elements * sizeof(data_t), MS_SYNC);

    if (dump)
    {
        const char *txt_filename = "./sorted_matrix.txt";
        FILE *txt_file = fopen(txt_filename, "w");

        if (txt_file == NULL)
        {
            perror("Error al crear el archivo de texto");
            munmap(mapped_binary_file, number_of_elements * sizeof(data_t));
            exit(EXIT_FAILURE);
        }

        for (i = 0; i < number_of_elements; i++)
        {
            fprintf(txt_file, "%lf\n", mapped_binary_file[i]);
        }

        fclose(txt_file);
    }

    munmap(mapped_binary_file, number_of_elements * sizeof(data_t));

    exit(EXIT_SUCCESS);
}