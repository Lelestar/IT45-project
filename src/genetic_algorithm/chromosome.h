#ifndef CHROMOSOME_H
#define CHROMOSOME_H

#include <math.h>
#include <stdio.h>
#include <iostream>
#include "random.h"

// The Chromosome class represents the structure of a solution to the problem
class Chromosome {
public:
    // ATTRIBUTES
    int *genes;         // the genes of the chromosome/solution
    int size;           // the size of the chromosome = number of genes
    int fitness;        // the value of the objective function (fitness) of the solution

    // CONSTRUCTORS
    Chromosome(int cs); // constructor of the object randomly
    ~Chromosome();      // destructor of the object

    // METHODS
    void display();    // function to display the chromosome (i.e., the solution)
    void evaluate(int **distance);
                       // function to evaluate the chromosome (i.e., calculate the fitness)
                       // It should be launched at the creation of the solution and after
                       // the execution of mutation and crossover operators
    void sort();       // orders the sequence if gene[1] > gene[size - 1]
    void copy(Chromosome* source);  // copies the 'source' chromosome
    bool identical(Chromosome* chrom); // tests if 2 chromosomes are identical

    // MUTATION OPERATORS
    void swap_2_genes(int gene1,     // interchange 2 genes of the chromosome
                      int gene2);
    void swap_2_consecutive_genes(); // interchange 2 consecutive genes of the chromosome
    void swap_2_random_genes();      // randomly interchange 2 genes of the chromosome
    void move_1_gene();              // moves one gene in the chromosome
    void invert_sequence_of_genes(); // inverts a sequence of genes in the chromosome
};

# endif
