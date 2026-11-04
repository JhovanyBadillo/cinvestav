/*
Programa que lee el primer numero como double de one_line.txt
*/

#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *file = fopen("one_line.txt", "rb");

    char line[90];

    fgets(line, sizeof(line), file);

    char *endptr;

    double first = strtod(line, &endptr);

    printf("first: %lf\n", first);
}