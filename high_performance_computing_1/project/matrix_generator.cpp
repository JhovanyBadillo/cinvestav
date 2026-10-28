#include <print>
#include <fstream>

#include "rng.h"

std::ofstream file("random_matrix.txt");

int main()
{
    size_t i, j, N = 10;

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            file << getUniformRandom();

            if (j <= (N - 2))
            {
                file << " ";
            }
        }

        file << std::endl;
    }

    file.close();

    return 0;
}
