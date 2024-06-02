#include "solution.h"

using namespace std;

/**
 * Initializes a solution: a cycle passing through all cities.
 * @param nv Number of cities
 */
Solution::Solution(int nv) : size(nv) {
    city = new int[size];
    // Arbitrarily, we always start with city 0
    city[0] = 0;
    for(int i = 1; i < size; i++) {
        bool repeat = true;
        int a;
        while(repeat) {
            repeat = false;
            // Randomly choose the next city
            a = Random::random(size);
            // The city must not already be in the cycle
            // if so, we repeat
            for (int j = 0; j < i; j++)
                if (a == city[j])
                    repeat = true;
        }
        city[i] = a;
    }
    // Arbitrarily impose that city[1] > city[size-1]
    order();
}

/**
 * Copies the cities of a solution. The fitness is not copied.
 * @param source Source solution to copy
 * @return Reference to the copied solution
 */
Solution& Solution::operator=(const Solution& source) {
    if (this->size != source.size) {
        if (city) delete[] city;
        size = source.size;
        city = new int[size];
    }
    for(int i = 0; i < size; i++)
        city[i] = source.city[i];
    return *this;
}

/**
 * Destructor
 */
Solution::~Solution() {
    if (city) delete[] city;
}

/**
 * Equality operator: tests if two solutions are identical
 * @param sol Solution to compare
 * @return true if solutions are identical, false otherwise
 */
bool Solution::operator==(const Solution& sol) {
    for(int i = 1; i < size; i++)
        if (sol.city[i] != this->city[i])
            return false;
    return true;
}

/**
 * Evaluates a solution: it is the sum of the distances connecting the cities.
 * @param distance Distance matrix
 */
void Solution::evaluate(int **distance) {
    fitness = 0;
    for(int i = 0; i < size - 1; i++)
        fitness += distance[city[i]][city[i + 1]];
    fitness += distance[city[0]][city[size - 1]];
}

/**
 * Displays the solution.
 */
void Solution::display() {
    for(int i = 0; i < size; i++)
        cout << city[i] << "-";
    cout << "--> " << fitness << " km" << endl;
}

/**
 * Orders the tour direction if city[1] > city[size-1].
 */
void Solution::order() {
    int temp, k;
    // Place city "0" at the head of the solution (city[0])
    if (city[0] != 0) {
        int position_0 = 0;
        int * city_copy = new int[size];
        for (int i = 0; i < size; i++) {
            city_copy[i] = city[i];
            if (city[i] == 0)
                position_0 = i;
        }
        k = 0;
        for (int i = position_0; i < size; i++) {
            city[k] = city_copy[i];
            k++;
        }
        for (int i = 0; i < position_0; i++) {
            city[k] = city_copy[i];
            k++;
        }
        delete[] city_copy;
    }

    // The number of the 2nd city must be smaller than that of the last city
    if (city[1] > city[size - 1]) {
        for(int k = 1; k <= 1 + (size - 2) / 2; k++) {
            temp = city[k];
            city[k] = city[size - k];
            city[size - k] = temp;
        }
    }
}

/**
 * Swaps two cities in the solution.
 * @param city1 First city
 * @param city2 Second city
 */
void Solution::swap(int city1, int city2) {
    int temp = city[city1];
    city[city1] = city[city2];
    city[city2] = temp;
}

/**
 * Reverses the order of cities between two indices.
 * @param start Start index
 * @param end End index
 */
void Solution::reverse_segment(int start, int end) {
    while (start < end) {
        swap(city[start], city[end]);
        start++;
        end--;
    }
}