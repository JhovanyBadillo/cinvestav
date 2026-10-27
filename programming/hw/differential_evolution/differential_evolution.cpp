#include <print>
#include <vector>

#include "operation.h"
#include "individual.h"
#include "population.h"

#define Np 20

int testing()
{
    Population natives(Np, 0.9, rastrigin);

    natives.initialize();

    for (individual_t element : natives.getIndividuals())
    {
        std::println("x = {0}, y = {1}, xy = {2}",
                     element.parameters.singles[0], element.parameters.singles[1], element.parameters.xy);
    }

    std::println("{0}", toReal(2, 0));

    natives.mutate();

    std::println("mutants");

    for (individual_t mutant : natives.getMutants())
    {
        std::println("x = {}, y = {}, xy = {}",
                     mutant.parameters.singles[0], mutant.parameters.singles[1], mutant.parameters.xy);
    }

    natives.doCrossover(0.1);

    std::println("trials");

    for (individual_t trial : natives.getTrials())
    {
        std::println("x = {}, y = {}, xy = {}",
                     trial.parameters.singles[0], trial.parameters.singles[1], trial.parameters.xy);
    }

    std::println("selection");

    natives.doSelection();

    std::println("new generation");

    for (individual_t individual : natives.getIndividuals())
    {
        std::println("x = {}, y = {}, xy = {}",
                     individual.parameters.singles[0], individual.parameters.singles[1], individual.parameters.xy);
    }

    std::println("fitnesses");

    for (individual_t individual : natives.getIndividuals())
    {
        std::println("{}",
                     rastrigin(toReal(individual.parameters.singles[0], 0), toReal(individual.parameters.singles[1], 1)));
    }

    return 0;
}

int main()
{
    size_t MAX_GENERATIONS = 100, g;
    double Cr = 0.9;

    Population natives(Np, 0.5, rastrigin);

    natives.initialize();

    for (g = 0; g < MAX_GENERATIONS; g++)
    {
        natives.mutate();
        natives.doCrossover(Cr);
        natives.doSelection();
    }

    std::println("last generation fitnesses");

    for (individual_t individual : natives.getIndividuals())
    {
        std::println("x = {}, y = {}, fitness = {}",
                     toReal(individual.parameters.singles[0], 0),
                     toReal(individual.parameters.singles[1], 1),
                     rastrigin(toReal(individual.parameters.singles[0], 0), toReal(individual.parameters.singles[1], 1)));
    }
}