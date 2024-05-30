#ifndef SOLUTION_H
#define SOLUTION_H

#include <cmath>
#include <cstdio>
#include <iostream>
#include "random.h"

/**
 * The Solution class represents the structure
 * of a solution for the Traveling Salesman Problem.
 */
class Solution {
public:
    // ATTRIBUTES
    int *city;                  ///< Solution variables
    int size;                   ///< Size of the solution
    int fitness;                ///< Fitness value of the solution = length of the tour

    /**
     * Constructor
     * @param nv Number of cities
     */
    Solution(int nv);

    /**
     * Destructor
     */
    ~Solution();

    /**
     * Assignment operator: copies the solution 'source'
     * @param source Source solution to copy
     * @return Reference to the copied solution
     */
    Solution& operator=(const Solution& source);

    /**
     * Equality operator: tests if two solutions are identical
     * @param sol Solution to compare
     * @return true if solutions are identical, false otherwise
     */
    bool operator==(const Solution& sol);

    /**
     * Evaluates the solution (i.e., calculates the fitness)
     * @param distance Distance matrix
     */
    void evaluate(int **distance);

    /**
     * Displays the solution
     */
    void display();

    /**
     * Orders the tour direction if city[1] > city[size-1]
     */
    void order();

    /**
     * Swaps two cities in the solution
     * @param city1 First city
     * @param city2 Second city
     */
    void swap(int city1, int city2);

    /**
     * Reverses the order of cities between two indices
     * @param start Start index
     * @param end End index
     */
    void reverse_segment(int start, int end);
};

#endif
