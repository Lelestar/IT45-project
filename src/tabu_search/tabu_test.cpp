#include <iostream>
#include <vector>
#include <chrono>
#include <limits>
#include "tabu_search.h"

using namespace std;

struct TestResult {
    int max_no_improvement_iterations;
    int num_perturbations_denominator;
    double average_fitness;
};

void run_test(int max_no_improvement_iterations, int num_perturbations_denominator, const string& filename, int num_iterations, int num_tests, vector<TestResult>& results) {
    double total_best_fitness = 0.0;

    for (int test = 0; test < num_tests; ++test) {
        TabuSearch ts(num_iterations, 80, filename, 0, -1);
        ts.setMaxNoImprovementIterations(max_no_improvement_iterations);
        ts.setNumPerturbationsDenominator(num_perturbations_denominator);

        Solution* best_solution = ts.optimize();

        cout << "Test " << test + 1 << ": Best fitness = " << best_solution->fitness << endl;

        total_best_fitness += best_solution->fitness;

        delete best_solution;
    }

    double average_fitness = total_best_fitness / num_tests;
    cout << "Average best fitness for max_no_improvement_iterations = " << max_no_improvement_iterations
         << " and num_perturbations_denominator = " << num_perturbations_denominator << ": " << average_fitness << endl;

    results.push_back({max_no_improvement_iterations, num_perturbations_denominator, average_fitness});
}

int main() {
    string filename = "../../data/berlin52.tsp";
    int num_iterations = 1500;
    int num_tests = 10;

    vector<int> max_no_improvement_iterations_values = {50, 100, 150, 200};
    vector<int> num_perturbations_denominator_values = {3, 4, 5, 6};

    vector<TestResult> results;

    for (int max_no_improvement_iterations : max_no_improvement_iterations_values) {
        for (int num_perturbations_denominator : num_perturbations_denominator_values) {
            run_test(max_no_improvement_iterations, num_perturbations_denominator, filename, num_iterations, num_tests, results);
        }
    }

    // Trouver la meilleure combinaison
    TestResult best_result = results[0];
    for (const auto& result : results) {
        if (result.average_fitness < best_result.average_fitness) {
            best_result = result;
        }
    }

    cout << "Best combination: max_no_improvement_iterations = " << best_result.max_no_improvement_iterations
         << ", num_perturbations_denominator = " << best_result.num_perturbations_denominator
         << ", with average fitness = " << best_result.average_fitness << endl;

    return 0;
}
