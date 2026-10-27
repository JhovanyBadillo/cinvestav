#ifndef OPERATION_H
#define OPERATION_H

#include <array>
#include <cmath>
#include <cstdint>
#include <algorithm>

#include "rng.h"

const std::array<double, 2> UPPER_BOUNDS = {5.12F, 5.12F};
const std::array<double, 2> LOWER_BOUNDS = {-5.12F, -5.12F};

double toReal(uint32_t integer, size_t dimension)
{
    return LOWER_BOUNDS[dimension] +
           (static_cast<double>(integer) / static_cast<double>(UINT32_MAX)) *
               (UPPER_BOUNDS[dimension] - LOWER_BOUNDS[dimension]);
}

uint32_t toInteger(double real, size_t dimension)
{
    real = std::clamp(real, LOWER_BOUNDS[dimension], UPPER_BOUNDS[dimension]);

    return static_cast<uint32_t>(round(
        (real - LOWER_BOUNDS[dimension]) * static_cast<double>(UINT32_MAX) /
        (UPPER_BOUNDS[dimension] - LOWER_BOUNDS[dimension])));
}

uint32_t getRandomParameter(size_t dimension)
{
    double randomParameter = getUniformRandom() *
                                 (UPPER_BOUNDS[dimension] - LOWER_BOUNDS[dimension]) +
                             LOWER_BOUNDS[dimension];

    return toInteger(randomParameter, dimension);
}

double rastrigin(double x, double y)
{
    double pi = (double)M_PI;

    double c = 20.0 + (x * x - 10.0 * cos(2 * pi * x)) + (y * y - 10.0 * cos(2 * pi * y));

    return c;
}

double booth(double x, double y)
{
    double A = x + 2.0 * y - 7.0;
    double B = 2.0 * x + y - 5.0;

    return A * A + B * B;
}

double sphere(double x, double y)
{
    return (x - 1) * (x - 1) + y * y;
}

#endif // OPERATION_H