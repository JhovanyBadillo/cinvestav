#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <cstdint>

typedef struct individual individual_t;
typedef union parameters parameters_t;

union parameters
{
    uint64_t xy;
    uint32_t singles[2];
};

struct individual
{
    parameters_t parameters;
    double fitness;
};

#endif // INDIVIDUAL_H
