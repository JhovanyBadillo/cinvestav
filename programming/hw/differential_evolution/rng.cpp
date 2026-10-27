#include <random>

#include "rng.h"

double getUniformRandom()
{
    std::random_device device;
    std::mt19937 gen(device());
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    return (double)distribution(gen);
}

size_t chooseRandom()
{
    std::random_device device;
    std::mt19937 gen(device());

    std::uniform_int_distribution<> distribution(0, 1);

    return (size_t)distribution(gen);
}