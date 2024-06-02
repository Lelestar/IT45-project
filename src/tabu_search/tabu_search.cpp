#include "tabu_search.h"

using namespace std;


/**
 * @brief Load the coordinates of cities from a TSP file
 * 
 * @param file_path path to the TSP file
 * @param cities vector to store the coordinates of cities
 * @return the dimension (number of cities)
 */ 
int load_tsp_coordinates(const string& file_path, vector<City>& cities) {
    ifstream file(file_path);
    if (!file) {
        cerr << "Unable to load the file " << file_path << "\n";
        exit(1);
    }

    string line;
    int dimension = 0;

    // Read the dimension and coordinates
    while (getline(file, line)) {
        if (line.find("DIMENSION") != string::npos) {
            istringstream iss(line);
            string temp;
            while (iss >> temp) {
                if (isdigit(temp[0])) {
                    dimension = stoi(temp);
                    break;
                }
            }
        }
        if (line == "NODE_COORD_SECTION") {
            break;
        }
    }

    cities.resize(dimension);

    int id;
    double x, y;
    for (int i = 0; i < dimension; ++i) {
        file >> id >> x >> y;
        cities[id - 1] = {x, y}; // Make sure the IDs start from 1
    }

    file.close();
    return dimension;
}

/**
 * @brief Calculate the Euclidean distance between two cities
 * 
 * @param a city a
 * @param b city b
 * @return the Euclidean distance between a and b
 */
double euclidean_distance(const City& a, const City& b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}


/**
 * Constructor: Initializes the parameters of Tabu Search and generates the initial solution.
 * Initializes the tabu list.
 */
TabuSearch::TabuSearch(int nbiter, int dt, const string& filename, int verbose, int max_duration_seconds) {
    num_iterations = nbiter;
    current_iter = 0;
    tabu_duration = dt; 
    best_evaluation = INT_MAX;
    this->verbose = verbose;
    this->max_duration_seconds = max_duration_seconds;
    no_improvement_counter = 0;
    max_no_improvement_iterations = 50;
    num_perturbations_denominator = 6;
    vector<City> cities;
    solution_size = load_tsp_coordinates(filename, cities); // Load city coordinates
    constructDistance(solution_size, cities); // Construct distance matrix
    current = new Solution(solution_size);
    current->evaluate(distances);

    tabu_list = new int*[solution_size];
    for(int i = 0; i < solution_size; i++) {
        tabu_list[i] = new int[solution_size];
        for(int j = 0; j < solution_size; j++)
            tabu_list[i][j] = -1;
    }

    cout << "The initial random solution is: ";
    current->display();

    tabu_list2 = new int*[tabu_duration];
    for(int i = 0; i < tabu_duration; i++) {
        tabu_list2[i] = new int[solution_size];
        for(int j = 0; j < solution_size; j++)
            tabu_list2[i][j] = -1;
    }
}

/**
 * Destructor: Frees allocated memory.
 */
TabuSearch::~TabuSearch() {
    delete current;
    for(int i = 0; i < solution_size; i++) {
        delete[] tabu_list[i];
        delete[] distances[i];
    }
    for(int i = 0; i < tabu_duration; i++)
        delete[] tabu_list2[i];
    delete[] tabu_list;
    delete[] tabu_list2;
    delete[] distances;
}

/**
 * Checks if the file exists.
 * @param filename Name of the file
 * @return true if the file exists, false otherwise
 */
bool fileExists(const char* filename) {
    ifstream ifile(filename);
    return ifile.good();
}

/**
 * Constructs the distance matrix from the file.
 * @param nv Number of cities
 * @param cities Vector of cities
 */
void TabuSearch::constructDistance(int nv, const vector<City>& cities) {
    distances = new int*[nv];
    for(int i = 0; i < nv; i++)
        distances[i] = new int[nv];
    
    for (int i = 0; i < nv; i++) {
        for (int j = 0; j < nv; j++) {
            if (i == j) {
                distances[i][j] = 0.0;
            } else {
                distances[i][j] = euclidean_distance(cities[i], cities[j]);
            }
        }
    }
}

bool TabuSearch::notTabu(int i, int j, int fitness) {
    if(tabu_list[i][j] < current_iter || fitness < best_evaluation)
        return true;
    else
        return false;
}

bool TabuSearch::notTabu2(Solution* sol) {
    for(int i = 0; i < tabu_duration; i++) {
        for(int j = 1; j < solution_size; j++) {
            if (tabu_list2[i][j] != sol->city[j])
                j = solution_size;
            else if (j == solution_size - 1)
                return false;
        }
    }
    for(int i = 0; i < tabu_duration; i++) {
        for(int j = 1; j < solution_size; j++) {
            if (tabu_list2[i][j] != sol->city[solution_size - j])
                j = solution_size;
            else if (j == solution_size - 1)
                return false;
        }
    }
    return true;
}

void TabuSearch::updateTabuList2(Solution* sol, int& position) {
    if (tabu_duration != 0) {
        for(int j = 0; j < solution_size; j++)
            tabu_list2[position][j] = sol->city[j];
        position++;
        if (position == tabu_duration)
            position = 0;
    }
}

/**
 * Provides the best non-tabu neighbor using swap.
 * 
 * @param best_i Best city i
 * @param best_j Best city j
 */
void TabuSearch::neighborhoodSwap(int &best_i, int &best_j) {
    int best_neighbor;
    best_neighbor = 100000;

    // Select the first city for the move
    for(int i = 0; i < solution_size; i++) {
        // Select the second city for the move
        for(int j = i + 1; j < solution_size; j++) {
            if ( ((i != 0) || (j != solution_size - 1)) && ((i != 0) || (j != solution_size - 2)) ) {
                // Transform the current solution to the neighbor
                // using the move defined by the pair of cities
                current->swap(i, j);
                // Evaluate this neighbor
                current->evaluate(distances);
                // If this move is not tabu and
                // if this neighbor has the best fitness
                // then this neighbor becomes the best non-tabu neighbor
                if(notTabu(i, j, current->fitness) && current->fitness < best_neighbor) {
                    best_neighbor  = current->fitness;
                    best_i         = i;
                    best_j         = j;
                }
                // Re-transform this neighbor back to the current solution
                current->swap(i, j);
                // Re-evaluate the current solution
                current->evaluate(distances);
            }
        }
    }
}

/**
 * Provides the best non-tabu neighbor using 2-opt.
 * 
 * @param best_i Best city i
 * @param best_j Best city j
 */
void TabuSearch::neighborhood2opt(int &best_i, int &best_j) {
    int best_neighbor = 100000;

    for(int i = 0; i < solution_size - 1; i++) {
        for(int j = i + 1; j < solution_size; j++) {
            current->reverse_segment(i, j); // Perform 2-opt exchange
            current->evaluate(distances);

            if(notTabu(i, j, current->fitness) && current->fitness < best_neighbor) {
                best_neighbor = current->fitness;
                best_i = i;
                best_j = j;
            }

            current->reverse_segment(i, j); // Undo 2-opt exchange
        }
    }
}

/**
 * Perturbs the current solution by swapping a few cities.
 */
void TabuSearch::perturb_solution() {
    int num_perturbations = solution_size / num_perturbations_denominator;
    for (int i = 0; i < num_perturbations; ++i) {
        int city1 = rand() % solution_size;
        int city2 = rand() % solution_size;
        current->swap(city1, city2);
    }
    current->evaluate(distances);
}

void TabuSearch::setMaxNoImprovementIterations(int value) {
    max_no_improvement_iterations = value;
}

void TabuSearch::setNumPerturbationsDenominator(int value) {
    num_perturbations_denominator = value;
}



/**
 * Main procedure of the search
 */
Solution* TabuSearch::optimize() {
    bool first = true; // Indicates if it's the first time
    bool descent = false; // Indicates if the current solution is in a descent

    int f_before, f_after; // Fitness values before and after an iteration

    // The best solution found (= smallest minimum found) to keep
    Solution* best_solution = new Solution(solution_size);

    int best_i = 0; // The pair (best_i, best_j) represents the best non-tabu move
    int best_j = 0;
    best_evaluation = current->fitness;
    f_before = 10000000;

    // Counters for best solution updates and local minima
    int count_best_updates = 0;
    int count_local_minima = 0;

    // Timer start for max duration
    auto start_time = chrono::high_resolution_clock::now();

    // While the iteration limit is not reached
    for(current_iter = 0; current_iter < num_iterations; current_iter++) {
        // Check for max duration
        if (max_duration_seconds > 0) {
            auto current_time = std::chrono::high_resolution_clock::now();
            auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count();
            if (elapsed_time > max_duration_seconds) {
                cout << "Max duration reached. Stopping optimization." << endl;
                break;
            }
        }

        neighborhoodSwap(best_i, best_j); // Get the best non-tabu move
        current->swap(best_i, best_j); // Move the current solution using this move
        //neighborhood2opt(best_i, best_j); // Get the best non-tabu move
        //current->reverse_segment(best_i, best_j); // Move the current solution using this move

        current->order(); // Reorder the solution starting from 0
        current->evaluate(distances); // Evaluate the new current solution

        f_after = current->fitness; // Fitness value after the move

        if(current->fitness < best_evaluation) { // If a smaller minimum is found
            best_evaluation = current->fitness; // Update best_evaluation
            *best_solution = *current; // Save the current solution as best_solution
            best_solution->evaluate(distances); // Evaluate the best solution
            count_best_updates++; // Increment the counter for best solution updates
            no_improvement_counter = 0; // Reset the no improvement counter
        } else { // If not in the smallest minimum found but in a local minimum
            // Local minimum detection criteria. Two cases:
            // 1. If the new solution is worse than the old one
            //    and we are performing a descent
            // 2. If the new solution is identical to the old one
            //    and it is the first time this happens
            no_improvement_counter++;
            if ( ((f_before < f_after) && (descent == true)) || ((f_before == f_after) && (first)) ) {
                if (verbose) {
                    cout << "We are in a local minimum at iteration "
                        << current_iter - 1 << " -> min = " << f_before
                        << " km (smallest local min found = "
                        << best_evaluation << " km)" << endl;
                }
                first = false;
                count_local_minima++;
            }

            if (f_before <= f_after) // The current solution is deteriorating
                descent = false;
            else
                descent = true; // The current solution is improving: descent

            if ((f_before != f_after) && (!first))
                first = true;

            // Si le compteur dépasse le seuil, applique une perturbation
            if (no_improvement_counter >= max_no_improvement_iterations) {
                if (verbose) {
                    cout << "Applying perturbation at iteration " << current_iter << endl;
                }
                perturb_solution();
                no_improvement_counter = 0;
            }
        }

        // Update the tabu list
        tabu_list[best_i][best_j] = current_iter + tabu_duration;
        f_before = f_after;

        // Output: index of iteration and the optimal solution so far
        if (verbose) {
            printf("%d\t%d\t%d\n", current_iter, current->fitness, best_evaluation);
        }
    }

    // Display the number of times the best solution was updated and the number of local minima visited
    if (verbose) {
        cout << "Number of best solution updates: " << count_best_updates << endl;
        cout << "Number of local minima visited: " << count_local_minima << endl;
    }

    return best_solution;
}
