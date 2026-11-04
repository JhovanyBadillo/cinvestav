/*
Programa que determina el tamaño en bytes del archivo one_line.txt
*/

#include <stdio.h>

int main()
{
    FILE *file = fopen("one_line.txt", "rb");

    fseek(file, 0, SEEK_END);

    long size = ftell(file);

    printf("file size: %ld\n", size);

    fclose(file);

    return 0;
}