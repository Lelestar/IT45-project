#ifndef _AE_H
#define _AE_H

#include <math.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <chrono>
#include <unordered_map>
#include <unordered_set>
#include "population.h"
#include "chromosome.h"

using namespace std;

/**
 * This structure defines a city with its coordinates.
 */
struct City {
    double x;
    double y;
};

// The Ae class defines the parameters for running an evolutionary algorithm
//    as well as the main search procedure
class Ae {
public:
    // ATTRIBUTES
    int num_generations;     // number of generations after which the search is stopped
    int population_size;     // number of individuals in the population
    double crossover_rate;   // crossover rate: value between 0 and 1
    double mutation_rate;    // mutation rate: value between 0 and 1
    int chromosome_size;     // number of genes in the chromosome
    Population *pop;         // list of individuals in the population
    int **distances;         // distance matrix between cities
    int verbose;             // verbose output
    int max_duration_seconds; // maximum duration in seconds

    // CONSTRUCTORS
    Ae(int ng, int ps, double cr, double mr, char* file_name, int v, int mds);
                             // constructor of the Evolutionary Algorithm object
    ~Ae();                   // destructor of the Ae object

    // METHODS
    void construct_distance_matrix(int dimension, vector<City>& cities);
                             // constructs the distance matrix
    Chromosome* optimize();  // main function that directs the evolutionary algorithm

    // CROSSOVER OPERATORS
    // 1X crossover operator for two chromosomes
    void crossover1X(Chromosome* parent1, Chromosome* parent2,
                     Chromosome* child1, Chromosome* child2);
    // 2X crossover operator for two chromosomes
    void crossover2X(Chromosome* parent1, Chromosome* parent2,
                     Chromosome* child1, Chromosome* child2);
    // 2LOX crossover operator for two chromosomes
    void crossover2LOX(Chromosome* parent1, Chromosome* parent2,
                       Chromosome* child1, Chromosome* child2);
    // PMX crossover operator for two chromosomes
    void crossoverPMX(Chromosome* parent1, Chromosome* parent2,
                      Chromosome* child1, Chromosome* child2);
};

# endif
