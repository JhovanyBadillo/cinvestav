#ifndef POPULATION_H
#define POPULATION_H

#include <vector>
#include <functional>

#include "individual.h"

class Population
{
public:
    Population(
        uint32_t size,
        double mutationFactor,
        std::function<double(double, double)> targetFunction);
    void initialize();
    std::vector<individual_t> getIndividuals();
    void mutate();
    std::vector<individual_t> getMutants();
    void doCrossover(double Cr);
    std::vector<individual_t> getTrials();
    void computeTrialsFitness();
    void doSelection();

private:
    double F;
    uint32_t Np;
    std::vector<individual_t> individuals;
    std::function<double(double, double)> targetFunction;
    std::vector<individual_t> mutants;
    std::vector<individual_t> trials;
};

#endif // POPULATION_H
