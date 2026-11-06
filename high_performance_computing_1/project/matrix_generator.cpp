#include <iostream>
#include <fstream>

#include "rng.h"

std::ofstream file("random_matrix_10.bin", std::ios::binary);

int main()
{
    size_t i, j, N = 10;
    double number;

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            number = getUniformRandom();

            file.write((char *)&number, sizeof number);
        }
    }

    file.close();

    return 0;
}
