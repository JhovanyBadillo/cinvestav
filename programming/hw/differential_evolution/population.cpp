#include <cmath>
#include <print>
#include <functional>

#include "population.h"
#include "operation.h"
#include "rng.h"

Population::Population(uint32_t size, double mutationFactor, std::function<double(double, double)> targetFunction)
    : F(mutationFactor), Np(size), individuals(size), targetFunction(targetFunction) {}

void Population::initialize()
{
    for (individual_t &element : individuals)
    {
        element.parameters.singles[0] = getRandomParameter(0);
        element.parameters.singles[1] = getRandomParameter(1);
        element.fitness = targetFunction(
            toReal(element.parameters.singles[0], 0),
            toReal(element.parameters.singles[1], 1));
    }
}

std::vector<individual_t> Population::getIndividuals()
{
    return individuals;
}

void Population::mutate()
{
    size_t i;

    size_t r0, r1, r2;
    double x, y;
    individual_t mutant;

    mutants.clear();
    mutants.reserve(Np);

    for (i = 0; i < Np; i++)
    {
        do
        {
            r0 = (size_t)floor(getUniformRandom() * Np);
        } while (r0 == i);

        do
        {
            r1 = (size_t)floor(getUniformRandom() * Np);
        } while (r1 == r0 || r1 == i);

        do
        {
            r2 = (size_t)floor(getUniformRandom() * Np);
        } while (r2 == r1 || r2 == r0 || r2 == i);

        x = toReal(individuals[r0].parameters.singles[0], 0) +
            F * (toReal(individuals[r1].parameters.singles[0], 0) - toReal(individuals[r2].parameters.singles[0], 0));

        y = toReal(individuals[r0].parameters.singles[1], 1) +
            F * (toReal(individuals[r1].parameters.singles[1], 1) - toReal(individuals[r2].parameters.singles[1], 1));

        mutant.parameters.singles[0] = toInteger(x, 0);
        mutant.parameters.singles[1] = toInteger(y, 1);

        mutants.push_back(mutant);
    }
}

std::vector<individual_t> Population::getMutants()
{
    return mutants;
}

void Population::doCrossover(double Cr)
{
    size_t i, j, jrandom, D = 2;

    double r;

    individual_t trial;

    trials.clear();
    trials.reserve(Np);

    for (i = 0; i < Np; i++)
    {
        jrandom = chooseRandom();

        for (j = 0; j < D; j++)
        {
            r = getUniformRandom();

            if (r <= Cr || j == jrandom)
            {
                trial.parameters.singles[j] = mutants[i].parameters.singles[j];
            }
            else
            {
                trial.parameters.singles[j] = individuals[i].parameters.singles[j];
            }
        }

        trials.push_back(trial);
    }
}

std::vector<individual_t> Population::getTrials()
{
    return trials;
}

void Population::computeTrialsFitness()
{
    size_t i;

    for (i = 0; i < Np; i++)
    {
        trials[i].fitness = targetFunction(
            toReal(trials[i].parameters.singles[0], 0),
            toReal(trials[i].parameters.singles[1], 1));
    }
}

void Population::doSelection()
{
    computeTrialsFitness();

    size_t i;

    for (i = 0; i < Np; i++)
    {
        if (trials[i].fitness <= individuals[i].fitness)
        {
            individuals[i].parameters.xy = trials[i].parameters.xy;
            individuals[i].fitness = trials[i].fitness;
        }
    }
}