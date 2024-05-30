#include <iostream>
#include <fstream>
#include <cmath>
#include <cstring>
#include <cstdio>
#include "random.h"
#include "tabu_search.h"
#include "solution.h"

using namespace std;

/**
 * Main function to execute the Tabu Search algorithm for solving the TSP.
 * 
 * @param argc number of parameters
 * @param argv array containing the parameters
 * Either the executable 'tabu_algo' takes no arguments or it takes 4 arguments:
 *   1. number of iterations (stopping criterion of the algorithm)
 *   2. length of the Tabu list
 *   3. number of cities
 *   4. filename indicating the distances between cities
 */
int main(int argc, char **argv) {
    // Initialize the random number generator
    Random::randomize();

    // Default values
    int num_iterations = 10;
    int tabu_length = 0;
    int num_cities = 10;
    char distanceFile[100];
    strcpy(distanceFile, "data/distances_between_cities_10.txt");

    cout << "Synopsis: " << argv[0] << " num_iter tabu_length number_cities distance_file " << endl;
    cout << "   1. num_iter      : number of iterations, default value = " << num_iterations << endl;
    cout << "   2. tabu_length   : tabu list length, default value = " << tabu_length << endl;
    cout << "   3. number_cities : number of cities in the instance, default value = " << num_cities << endl;
    cout << "   4. distance_file : distance matrix filename, default value = " << distanceFile << endl;
    cout << endl;

    if (argc == 5) {
        num_iterations = atoi(argv[1]);
        tabu_length = atoi(argv[2]);
        num_cities = atoi(argv[3]);
        strcpy(distanceFile, argv[4]);
    } else if (argc != 1) {
        cout << "Incorrect arguments" << endl;
        exit(EXIT_FAILURE);
    }

    // Initialize the parameters of the TabuSearch and create the initial solution
    //   1st parameter: number of iterations (stopping criterion of the algorithm)
    //   2nd parameter: length of the Tabu list
    //   3rd parameter: number of cities
    //   4th parameter: file containing the distances between cities    
    TabuSearch algo(num_iterations, tabu_length, num_cities, distanceFile);

    // Run the search with the Tabu method
    Solution* best = algo.optimize();
    // Display the best solution found
    cout << endl << "The best solution found is: ";
    best->display();

    delete best;

    return 0;
}
