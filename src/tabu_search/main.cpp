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
    int num_iterations = 1500;
    int tabu_length = 80;
    char tspFile[100];
    int verbose = 0;
    int max_duration_seconds = -1;
    strcpy(tspFile, "data/berlin52.tsp");

    cout << "Synopsis: " << argv[0] << " num_iter tabu_length number_cities distance_file " << endl;
    cout << "   1. num_iter      : number of iterations, default value = " << num_iterations << endl;
    cout << "   2. tabu_length   : tabu list length, default value = " << tabu_length << endl;
    cout << "   3. tsp_file      : TSP file containing the coordinates of cities, default value = " << tspFile << endl;
    cout << "   4. verbose             : verbose output, default value = " << verbose << endl;
    cout << "   5. max_duration_seconds: maximum duration in seconds, default value = " << max_duration_seconds << endl;
    cout << endl;

    if (argc >= 4) {
        num_iterations = atoi(argv[1]);
        tabu_length = atoi(argv[2]);
        strcpy(tspFile, argv[3]);
        if (argc >= 5) {
            // String comparison to check if the verbose flag is set
            if (strcmp(argv[4], "verbose") == 0) {
                verbose = 1;
            }
        }
        if (argc == 6) {
            max_duration_seconds = atoi(argv[5]);
            num_iterations = 1000000000; // Set a large number of iterations
        }
    } else if (argc != 1) {
        cout << "Incorrect arguments" << endl;
        exit(EXIT_FAILURE);
    }
    cout << "Results of the Tabu Search algorithm for the TSP instance " << tspFile << "\n\n";
    // Initialize the parameters of the TabuSearch and create the initial solution
    //   1st parameter: number of iterations (stopping criterion of the algorithm)
    //   2nd parameter: length of the Tabu list
    //   3rd parameter: number of cities
    //   4th parameter: file containing the distances between cities    
    TabuSearch algo(num_iterations, tabu_length, tspFile, verbose, max_duration_seconds);

    // Run the search with the Tabu method
    Solution* best = algo.optimize();
    // Display the best solution found
    cout << endl << "The best solution found is: ";
    best->display();

    delete best;

    return 0;
}
