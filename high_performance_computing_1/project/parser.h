/*
This program takes as input the data as it comes from a text file
and maps it to virtual memory.
*/
#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "types.h"

void parse(
    const char *text_filename,
    const char *binary_filename,
    data_t **mapped_binary_file,
    size_t *number_of_elements)
{
    int text_file = open(text_filename, O_RDONLY);
    if (text_file == -1)
    {
        perror("cannot open data text file");
        exit(EXIT_FAILURE);
    }

    struct stat file_stats;
    if (fstat(text_file, &file_stats) == -1)
    {
        perror("cannot determine text file size");
        close(text_file);
        exit(EXIT_FAILURE);
    }

    size_t text_file_size = file_stats.st_size;

    char *mapped_text_file = mmap(NULL, text_file_size, PROT_READ, MAP_PRIVATE, text_file, 0);
    if (mapped_text_file == MAP_FAILED)
    {
        perror("cannot map text file");
        close(text_file);
        exit(EXIT_FAILURE);
    }

    close(text_file);

    int binary_file = open(binary_filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (binary_file == -1)
    {
        perror("cannot create binary data file");
        exit(EXIT_FAILURE);
    }

    /* initially the binary has the same size of the text file.
    Then we truncate to its real size. */
    size_t binary_file_size = text_file_size;

    /* binary_file is filled with zeros */
    ftruncate(binary_file, binary_file_size);

    *mapped_binary_file = mmap(NULL, binary_file_size, PROT_READ | PROT_WRITE, MAP_SHARED, binary_file, 0);
    if (*mapped_binary_file == MAP_FAILED)
    {
        perror("mmap failed");
        close(binary_file);
        exit(EXIT_FAILURE);
    }

    /* now we parse number by number */
    char *ptr_to_text = mapped_text_file;
    char *limit = mapped_text_file + text_file_size;
    size_t count_of_floats = 0;
    float number;

    while (ptr_to_text < limit)
    {
        char *end_ptr;

        number = strtof(ptr_to_text, &end_ptr);

        if (ptr_to_text == end_ptr)
        {
            ptr_to_text++;
            continue;
        }

        (*mapped_binary_file)[count_of_floats] = number;
        count_of_floats++;
        ptr_to_text = end_ptr;
    }

    printf("Parsing finished. To sort: %ld numbers\n", count_of_floats);

    size_t real_binary_file_size = count_of_floats * sizeof(float);

    msync(*mapped_binary_file, real_binary_file_size, MS_SYNC);

    munmap(mapped_text_file, text_file_size);

    ftruncate(binary_file, real_binary_file_size);
    close(binary_file);

    *number_of_elements = count_of_floats;
}

#endif // PARSER_H