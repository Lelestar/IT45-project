#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstring>
#include <limits>
#include <sstream>
#include <chrono>
#include "utils.h"

using namespace std;

// City coordinates
vector<pair<double, double>> coordinates;

// Distance matrix
vector<vector<double>> dist;

// next_town[i] = next town after town i
vector<int> next_town;

vector<int> best_solution;
double best_eval = -1.0;

/**
 * @brief Print a matrix
 * 
 * @param d matrix to print
 */
void print_matrix(const vector<vector<double>>& d) {
    for (size_t i = 0; i < d.size(); ++i) {
        cout << i << ": ";
        for (size_t j = 0; j < d[i].size(); ++j) {
            cout << d[i][j] << " ";
        }
        cout << "\n";
    }
}

/**
 * @brief Compute the distance matrix
 * 
 * @param d distance matrix
 */
void compute_matrix(vector<vector<double>>& d) {
    size_t nbr_towns = coordinates.size();
    for (size_t i = 0; i < nbr_towns; ++i) {
        for (size_t j = 0; j < nbr_towns; ++j) {
            if (i == j) {
                d[i][j] = -1.0;
            } else {
                d[i][j] = sqrt((coordinates[i].first - coordinates[j].first) * (coordinates[i].first - coordinates[j].first) +
                                    (coordinates[i].second - coordinates[j].second) * (coordinates[i].second - coordinates[j].second));
            }
        }
    }
}

/**
 * @brief Print a solution
 * 
 * @param sol solution to print
 * @param eval evaluation of the solution
 */
void print_solution(const vector<int>& sol, double eval) {
    cout << "(" << eval << "): ";
    for (int town : sol) {
        cout << town << " ";
    }
    cout << "\n\n";
}

/**
 * @brief Evaluate a solution
 * 
 * @param sol solution to evaluate
 * @return evaluation of the solution
 */
double evaluation_solution(const vector<int>& sol) {
    double eval = 0;
    size_t nbr_towns = sol.size();
    for (size_t i = 0; i < nbr_towns; ++i) {
        int start = sol[i];
        int end = sol[(i + 1) % nbr_towns];
        eval += dist[start][end];
    }
    return eval;
}

/**
 * @brief Compute an initial solution based on the nearest neighbor heuristic
 * 
 * @return evaluation of the initial solution
 */
double initial_solution() {
    size_t nbr_towns = coordinates.size();
    vector<int> sol(nbr_towns);
    double eval = 0;
    vector<int> visited(nbr_towns, 0);
    int current_town = 0;
    visited[current_town] = 1;
    sol[0] = current_town;

    for (size_t i = 1; i < nbr_towns; ++i) {
        int next_town = -1;
        double min_dist = -1.0;
        for (size_t j = 0; j < nbr_towns; ++j) {
            if (visited[j] == 0 && (min_dist < 0 || dist[current_town][j] < min_dist)) {
                next_town = j;
                min_dist = dist[current_town][j];
            }
        }
        sol[i] = next_town;
        visited[next_town] = 1;
        current_town = next_town;
        eval += min_dist;
    }

    eval = evaluation_solution(sol);
    cout << "Initial solution ";
    print_solution(sol, eval);

    best_solution = sol;
    best_eval = eval;

    return eval;
}

/**
 * @brief Build a final solution
 */
void build_solution() {
    size_t nbr_towns = coordinates.size();
    vector<int> solution(nbr_towns);
    int indiceCour = 0;
    int villeCour = 0;

    while (indiceCour < nbr_towns) {
        solution[indiceCour] = villeCour;

        // Check if the cycle is not Hamiltonian
        for (int i = 0; i < indiceCour; ++i) {
            if (solution[i] == villeCour) {
                log_message("Cycle is not Hamiltonian");
                return;
            }
        }
        villeCour = next_town[villeCour];
        indiceCour++;
    }

    double eval = evaluation_solution(solution);
    if (best_eval < 0 || eval < best_eval) {
        best_eval = eval;
        best_solution = solution;
        cout << "New best solution: ";
        print_solution(solution, best_eval);
    }
}

/**
 * @brief Little algorithm
 * 
 * @param d0 distance matrix
 * @param iteration current iteration
 * @param eval_node_parent evaluation of the parent node
 */
void little_algorithm(vector<vector<double>> d0, int iteration, double eval_node_parent) {
    size_t nbr_towns = coordinates.size();
    if (iteration == nbr_towns) {
        build_solution();
        return;
    }

    // Copy distance matrix
    vector<vector<double>> d = d0;

    double eval_node_child = eval_node_parent;

    // Row reduction
    for (size_t i = 0; i < nbr_towns; ++i) {
        double min = numeric_limits<double>::infinity();
        for (size_t j = 0; j < nbr_towns; ++j) {
            if (d[i][j] >= 0) {
                min = std::min(min, d[i][j]);
            }
        }
        if (min != numeric_limits<double>::infinity()) {
            for (size_t j = 0; j < nbr_towns; ++j) {
                if (d[i][j] >= 0) {
                    d[i][j] -= min;
                }
            }
            eval_node_child += min;
        }
    }

    // Column reduction
    for (size_t j = 0; j < nbr_towns; ++j) {
        double min = numeric_limits<double>::infinity();
        for (size_t i = 0; i < nbr_towns; ++i) {
            if (d[i][j] >= 0) {
                min = std::min(min, d[i][j]);
            }
        }
        if (min != numeric_limits<double>::infinity()) {
            for (size_t i = 0; i < nbr_towns; ++i) {
                if (d[i][j] >= 0) {
                    d[i][j] -= min;
                }
            }
            eval_node_child += min;
        }
    }

    // Cutoff : stop if the evaluation is greater than the best evaluation
    if (best_eval >= 0 && eval_node_child >= best_eval) {
        return;
    }

    // Compute the penalties
    int izero = -1, jzero = -1;
    double max_penalty = -1.0;

    for (size_t i = 0; i < nbr_towns; ++i) {
        for (size_t j = 0; j < nbr_towns; ++j) {
            if (d[i][j] == 0) {
                double min_row = numeric_limits<double>::infinity();
                double min_col = numeric_limits<double>::infinity();
                for (size_t k = 0; k < nbr_towns; ++k) {
                    if (k != j && d[i][k] >= 0) {
                        min_row = min(min_row, d[i][k]);
                    }
                    if (k != i && d[k][j] >= 0) {
                        min_col = min(min_col, d[k][j]);
                    }
                }
                double penalty = (min_row == numeric_limits<double>::infinity() ? 0 : min_row) +
                                 (min_col == numeric_limits<double>::infinity() ? 0 : min_col);
                if (penalty > max_penalty) {
                    max_penalty = penalty;
                    izero = i;
                    jzero = j;
                }
            }
        }
    }

    // No zero in the matrix, solution infeasible
    if (izero == -1 || jzero == -1) {
        log_message("No zero in the matrix, solution infeasible");
        return;
    }

    // Branching
    vector<vector<double>> d2 = d;

    // Explore left branch
    // Block all paths leading from the chosen town and to the chosen town
    for (size_t i = 0; i < nbr_towns; ++i) {
        d2[izero][i] = -1;
        d2[i][jzero] = -1;
    }
    d2[jzero][izero] = -1;
    next_town[izero] = jzero;
    little_algorithm(d2, iteration + 1, eval_node_child);

    // Explore right branch
    d2 = d;
    d2[izero][jzero] = -1;
    little_algorithm(d2, iteration, eval_node_child);
}

/**
 * @brief Load a TSP file
 * 
 * @param file_path path to the TSP file
 */
void load_tsp_file(const string& file_path) {
    ifstream file(file_path);
    if (!file) {
        cerr << "Unable to load file " << file_path << "\n";
        exit(1);
    }

    string line;
    int dimension = 0;

    // Read the dimension and the coordinates
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
    
    coordinates.resize(dimension);
    dist.resize(dimension, vector<double>(dimension));
    next_town.resize(dimension);
    best_solution.resize(dimension);

    int id;
    double x, y;
    for (int i = 0; i < dimension; ++i) {
        file >> id >> x >> y;
        coordinates[i] = {x, y};
    }
    
    file.close();
}

/**
 * @brief Main function
 * 
 * @param argc number of arguments
 * @param argv arguments
 * @return int return code
 */
int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <tsp_instance_name>\n";
        return 1;
    }

    // Start the timer
    auto start_time = chrono::high_resolution_clock::now();

    string tsp_instance = argv[1];

    load_tsp_file(tsp_instance);

    cout << "Results of the Little algorithm for the TSP instance " << tsp_instance << "\n\n";
    compute_matrix(dist);
    cout << "Distance Matrix:\n";
    print_matrix(dist);
    cout << "\n";

    double initial_value = initial_solution();
    little_algorithm(dist, 0, 0.0);

    // Flush any remaining log messages
    flush_log();

    cout << "\nBest solution:";
    print_solution(best_solution, best_eval);

    // Stop the timer
    auto end_time = chrono::high_resolution_clock::now(); 
    auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
    cout << "Execution time: " << duration << " ms\n";

    return 0;
}
