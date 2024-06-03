#ifndef POPULATION_H
#define POPULATION_H

#include <math.h>
#include <stdio.h>
#include <iostream>
#include "chromosome.h"

// The Population class encompasses multiple potential solutions to the problem
class Population {
public:
    // ATTRIBUTES
    Chromosome **individuals; // list of individuals in the population
    int population_size;      // number of individuals in the population
    int *order;               // array giving the order of individuals in the population
                              // from best to worst based on fitness

    // CONSTRUCTORS
    Population(int ps, int cs); // constructor of the object
    ~Population();              // destructor of the object

    // METHODS
    void statistics(); // displays some statistics about the population
    int num_similar_chromosomes(Chromosome* chrom);
                        // counts the number of chromosomes similar to 'chrom'
    void similarity();  // displays the results of counting similar chromosomes
                        // to the best individuals in the population.

    // SELECTION AND REPLACEMENT OPERATORS
    Chromosome *roulette_selection();                 // biased roulette selection of an individual from the population
    void roulette_replacement(Chromosome *individual); // biased roulette replacement of an individual in the population with a given chromosome

    Chromosome *ranking_selection(float ranking_rate); // ranking selection of an individual from the population
    void ranking_replacement(Chromosome *individual, float ranking_rate); // ranking replacement of an individual in the population with a given chromosome

    Chromosome* random_selection();                // random selection and replacement
    void random_replacement(Chromosome* individual); // random replacement

    void sort();
    void resort();
    void display();
};

# endif
