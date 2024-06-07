#ifndef _TABU_SEARCH_H
#define _TABU_SEARCH_H

#include <cmath>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <chrono>
#include <climits>
#include "solution.h"

using namespace std;

/**
 * This structure defines a city with its coordinates.
 */
struct City {
    double x;
    double y;
};

/**
 * This class defines the parameters for running the Tabu Search
 * and the main search procedure.
 */
class TabuSearch {
    public:
        // ATTRIBUTES
        int current_iter;                ///< Current iteration
        int num_iterations;              ///< Number of iterations after which the search stops
        int tabu_duration;               ///< Tabu duration in number of iterations
        int solution_size;               ///< Number of cities in the solution
        Solution *current;               ///< Current solution managed by TabuSearch
        int **tabu_list2;                ///< List of tabu solutions
        int **tabu_list;                 ///< List of tabu durations associated with each pair of cities
        int **distances;                 ///< Distance matrix between cities
        int best_evaluation;             ///< Best evaluation found
        int verbose;                     ///< Verbose output flag
        int max_duration_seconds;        ///< Maximum duration in seconds
        int no_improvement_counter;      ///< Counter for the number of iterations without improvement
        int max_no_improvement_iterations; ///< Maximum number of iterations without improvement
        int num_perturbations_denominator; ///< Number of perturbations denominator              

        // CONSTRUCTORS
        /**
         * Constructs a TabuSearch instance.
         * 
         * @param nbiter Number of iterations
         * @param dt Tabu duration
         * @param filename Name of the TSP file
         */
        TabuSearch(int nbiter, int dt, const string& filename, int verbose = 0, int max_duration_seconds = -1);
        
        /**
         * Destructor for TabuSearch.
         */
        ~TabuSearch();

        // METHODS
        /**
         * Checks if the pair (city i, city j) is not tabu based on fitness.
         * 
         * @param i First city
         * @param j Second city
         * @param fitness Fitness value
         * @return true if the pair is not tabu, false otherwise
         */
        bool notTabu(int i, int j, int fitness);

        /**
         * Checks if the solution is not tabu.
         * 
         * @param sol Solution to check
         * @return true if the solution is not tabu, false otherwise
         */
        bool notTabu2(Solution* sol);

        /**
         * Provides the best non-tabu neighbor using swap.
         * 
         * @param best_i Best city i
         * @param best_j Best city j
         */
        void neighborhoodSwap(int& best_i, int& best_j);

        /**
         * Provides the best non-tabu neighbor using 2-opt.
         * 
         * @param best_i Best city i
         * @param best_j Best city j
         */
        void neighborhood2opt(int& best_i, int& best_j);

        /**
         * Constructs the distance matrix.
         * 
         * @param nv Number of cities
         * @param cities Vector of cities
         */
        void constructDistance(int nv, const vector<City>& cities);

        /**
         * Adds the solution to the tabu list at the specified position.
         * 
         * @param sol Solution to add
         * @param position Position in the tabu list
         */
        void updateTabuList2(Solution* sol, int& position);

        /**
         * Starts the Tabu Search.
         * 
         * @return Pointer to the best solution found
         */
        Solution* optimize();

        /**
         * Perturbs the solution by swapping a few cities.
         */
        void perturb_solution();

        void setMaxNoImprovementIterations(int value);
        void setNumPerturbationsDenominator(int value);
};

#endif
