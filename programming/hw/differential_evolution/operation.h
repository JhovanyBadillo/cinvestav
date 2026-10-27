#ifndef OPERATION_H
#define OPERATION_H

double toReal(uint32_t integer, size_t dimension);

uint32_t toInteger(double real, size_t dimension);

uint32_t getRandomParameter(size_t dimension);

double rastrigin(double x, double y);

double booth(double x, double y);

double sphere(double x, double y);

#endif // OPERATION_H