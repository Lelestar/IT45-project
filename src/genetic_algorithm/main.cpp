#include <math.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "ae.h"
#include "chromosome.h"
#include "random.h"

using namespace std;

int main(int argc, char **argv)
// argc: number of parameters
// argv: array containing the parameters
// The executable 'algo_evo' either takes no arguments or 6 arguments:
//    1. number of generations (integer > 0)
//    2. population size (integer > 0)
//    3. crossover rate (0 <= real <= 1)
//    4. mutation rate (0 <= real <= 1)
//    5. name of the file indicating the distances between cities
//    6. verbose (optional)
//    7. max_duration_seconds (optional)
{
    // initialize the random number generator
    Random::randomize();

    // default values
    int num_generations = 100000;
    int population_size = 100;
    float crossover_rate = 0.8;
    float mutation_rate = 0.5;
    char tsp_file[100];
    int verbose = 0;
    int max_duration_seconds = -1;
    strcpy(tsp_file, "data/berlin52.tsp");

    if (argc >= 6)
    {
        num_generations = atoi(argv[1]);
        population_size = atoi(argv[2]);
        crossover_rate = atof(argv[3]);
        mutation_rate = atof(argv[4]);
        strcpy(tsp_file, argv[5]);
        if (argc >= 7)
        {
            // string comparison to check if the verbose flag is set
            if (strcmp(argv[6], "verbose") == 0)
            {
                verbose = 1;
            }
        }
        if (argc == 8)
        {
            max_duration_seconds = atoi(argv[7]);
            num_generations = 1000000000; // set a large number of generations
        }
    }
    else if (argc == 1)
    {
        cout << "Default parameters" << endl;
    }
    else
    {
        cout << "Incorrect number of arguments." << endl;
        cout << "Either the executable 'algo_evo' takes no arguments or it takes 6 arguments: " << endl;
        cout << "   1. number of generations (integer > 0)" << endl;
        cout << "   2. population size (integer > 0)" << endl;
        cout << "   3. crossover rate (0 <= real <= 1)" << endl;
        cout << "   4. mutation rate (0 <= real <= 1)" << endl;
        cout << "   5. name of the TSP file" << endl;
        cout << "   6. verbose (optional)" << endl;
        cout << "   7. max_duration_seconds (optional)" << endl;
        exit(EXIT_FAILURE);
    }

    // initialize the evolutionary algorithm
    Ae algo(num_generations, population_size, crossover_rate, mutation_rate, tsp_file, verbose, max_duration_seconds);
    // arguments of the Ae object constructor
    //    1. number of generations (integer > 0)
    //    2. population size (integer > 0)
    //    3. crossover rate (0 <= real <= 1)
    //    4. mutation rate (0 <= real <= 1)
    //    5. TSP file
    //    6. verbose (optional)
    //    7. max_duration_seconds (optional)

    cout << "Results of the Genetic algorithm for the TSP instance " << tsp_file << "\n\n";
    
    // run the evolutionary algorithm
    Chromosome *best = algo.optimize();

    // display the fitness of the best individual found
    cout << "\nThe best solution found is: ";
    best->display();
}
