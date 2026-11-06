#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main()
{
    int file = open("random_matrix.bin", O_RDWR);

    double *file_in_memory = mmap(NULL, 10 * 10 * sizeof(double), PROT_READ | PROT_WRITE, MAP_SHARED, file, 0);

    for (size_t i = 0; i < 100; i++)
    {
        printf("%lf\n", file_in_memory[i]);
    }

    close(file);
}