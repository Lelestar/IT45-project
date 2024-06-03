#include "ae.h"

using namespace std;

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

double euclidean_distance(const City& a, const City& b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

// initialization of GA parameters and generation of the initial population
Ae::Ae(int nbg, int tp, double cr, double mr, char* file_name, int v, int mds)
{
    num_generations = nbg;
    population_size = tp;
    crossover_rate = cr;
    mutation_rate = mr;
    verbose = v;
    max_duration_seconds = mds;
    vector<City> cities;
    chromosome_size = load_tsp_coordinates(file_name, cities);
    construct_distance_matrix(chromosome_size, cities);
    pop = new Population(population_size, chromosome_size);
}

// destructor of the Ae object
Ae::~Ae()
{
    delete pop;
}

// main search procedure
Chromosome* Ae::optimize()
{
    int improvement = 0;
    Chromosome *child1 = new Chromosome(chromosome_size);
    Chromosome *child2 = new Chromosome(chromosome_size);
    Chromosome *parent1;
    Chromosome *parent2;
    int best_fitness;
    float ranking_rate = 1000.0;

    // evaluation of the individuals in the initial population
    for(int ind = 0; ind < population_size; ind++)
        pop->individuals[ind]->evaluate(distances);

    // sort the individuals based on their fitness
    pop->sort();

    best_fitness = pop->individuals[pop->order[0]]->fitness;
    
    // display initial population statistics
    cout << "Some statistics on the initial population" << endl;
    pop->statistics();
    cout << "\n\n";

    // Timer start for max duration
    auto start_time = chrono::high_resolution_clock::now();

    // while the limit on the number of generations is not reached
    for(int g = 0; g < num_generations; g++)
    {
        // Check for max duration
        if (max_duration_seconds > 0) {
            auto current_time = std::chrono::high_resolution_clock::now();
            auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count();
            if (elapsed_time > max_duration_seconds) {
                cout << "Max duration reached. Stopping optimization." << endl;
                break;
            }
        }

        // select two individuals from the current population
        parent1 = pop->roulette_selection();
        parent2 = pop->roulette_selection();
        //parent1 = pop->random_selection();
        //parent2 = pop->random_selection();
        //parent1 = pop->ranking_selection(ranking_rate); 
        //parent2 = pop->ranking_selection(ranking_rate);

        // perform crossover with probability "crossover_rate"
        if(Random::random(1000) / 1000.0 < crossover_rate)
        {
            crossover1X(parent1, parent2, child1, child2);    
            //crossover2X(parent1, parent2, child1, child2);
        }
        else
        {
            child1->copy(parent1);
            child2->copy(parent2);
        }

        // perform mutation on one child with probability "mutation_rate"
        if(Random::random(1000) / 1000.0 < mutation_rate)
            child1->swap_2_consecutive_genes();
            //child1->swap_2_random_genes();
            //child1->invert_sequence_of_genes();

        // perform mutation on the other child with probability "mutation_rate"
        if(Random::random(1000) / 1000.0 < mutation_rate)
            child1->swap_2_consecutive_genes();
            //child1->swap_2_random_genes();
            //child1->invert_sequence_of_genes();

        // evaluate the two newly generated individuals
        child1->evaluate(distances);
        child2->evaluate(distances);

        // insert the new individuals into the population
        pop->roulette_replacement(child1);
        pop->roulette_replacement(child2);
        //pop->random_replacement(child1);
        //pop->random_replacement(child2);
        //pop->ranking_replacement(child1, ranking_rate);
        //pop->ranking_replacement(child2, ranking_rate);

        // re-sort the population based on fitness
        pop->resort();

        // if one of the new solution-individuals is the best ever encountered
        if (pop->individuals[pop->order[0]]->fitness < best_fitness)
        {
            best_fitness = pop->individuals[pop->order[0]]->fitness;
            if (verbose) {
                cout << "Improvement of the best solution at generation " << g << " : " << best_fitness << endl;
            }
            improvement = g;
        }
    }
    // display final population statistics
    cout << "Some statistics on the final population" << endl;
    pop->statistics();
    // display the consanguinity of the final population
    pop->similarity();

    // return the best individual encountered during the search
    return pop->individuals[pop->order[0]];
}

// single point crossover operator: crossover1X
// 1) the 1X operator randomly chooses a crossover point
// 2) the 1X operator copies the beginning of parent 1 to the beginning of child 1
//                     and the beginning of parent 2 to the beginning of child 2.
// 3) the 1X operator completes child 1 with the missing genes by placing them in the order of parent 2
//                         and child 2 with the missing genes by placing them in the order of parent 1.
void Ae::crossover1X(Chromosome* parent1, Chromosome* parent2,
                     Chromosome* child1, Chromosome* child2)
{
    int num_genes = parent1->size;

    int* order_parent1 = new int[num_genes];
    int* order_parent2 = new int[num_genes];

    for (int i = 0; i < num_genes; i++)
    {
        order_parent1[parent1->genes[i]] = i;
        order_parent2[parent2->genes[i]] = i;
    }

    // 1) the 1X operator randomly chooses the crossover point
    int point = Random::random(num_genes);

    // 2) the 1X operator copies the beginning of parent 1 to the beginning of child 1
    //                     and the beginning of parent 2 to the beginning of child 2.
    child1->copy(parent1);
    child2->copy(parent2);

    // 3) the 1X operator completes child 1 with the missing genes by placing them in the order of parent 2
    //                         and child 2 with the missing genes by placing them in the order of parent 1.
    for (int k = point + 1; k < num_genes; k++)
    {
        for (int l = k + 1; l < num_genes; l++)
        {
            if(order_parent2[child1->genes[k]] > order_parent2[child1->genes[l]])
                child1->swap_2_genes(k, l);
            if(order_parent1[child2->genes[k]] > order_parent1[child2->genes[l]])
                child2->swap_2_genes(k, l);
        }
    }
    delete[] order_parent1;
    delete[] order_parent2;
}

// two-point crossover operator: crossover2X
// 1) the 2X operator randomly chooses 2 crossover points
// 2) the 2X operator copies the beginning of parent 1 to the beginning of child 1
//                        and the beginning of parent 2 to the beginning of child 2.
// 3) the 2X operator completes child 1 with the missing genes by placing them in the order of parent 2
//                         and child 2 with the missing genes by placing them in the order of parent 1.
void Ae::crossover2X(Chromosome* parent1, Chromosome* parent2,
                     Chromosome* child1, Chromosome* child2)
{
    int num_genes = parent1->size;

    int* order_parent1 = new int[num_genes];
    int* order_parent2 = new int[num_genes];

    for (int i = 0; i < num_genes; i++)
    {
        order_parent1[parent1->genes[i]] = i;
        order_parent2[parent2->genes[i]] = i;
    }

    // 1) the 2X operator randomly chooses two crossover points
    int point1 = Random::random(num_genes);
    int point2 = Random::random(num_genes);

    // ensure point1 < point2
    if (point1 > point2)
    {
        std::swap(point1, point2);
    }

    // 2) the 2X operator copies the beginning of parent 1 to the beginning of child 1
    // and the beginning of parent 2 to the beginning of child 2.
    child1->copy(parent1);
    child2->copy(parent2);

    // 3) complete child 1 with the missing genes by placing them in the order of parent 2
    // and child 2 with the missing genes by placing them in the order of parent 1.
    for (int k = point1 + 1; k < point2; k++)
    {
        for (int l = k + 1; l < point2; l++)
        {
            if (order_parent2[child1->genes[k]] > order_parent2[child1->genes[l]])
                child1->swap_2_genes(k, l);
            if (order_parent1[child2->genes[k]] > order_parent1[child2->genes[l]])
                child2->swap_2_genes(k, l);
        }
    }

    // complete the remaining parts after point2
    for (int k = point2; k < num_genes; k++)
    {
        for (int l = k + 1; l < num_genes; l++)
        {
            if (order_parent2[child1->genes[k]] > order_parent2[child1->genes[l]])
                child1->swap_2_genes(k, l);
            if (order_parent1[child2->genes[k]] > order_parent1[child2->genes[l]])
                child2->swap_2_genes(k, l);
        }
    }

    delete[] order_parent1;
    delete[] order_parent2;
}

void Ae::crossover2LOX(Chromosome* parent1, Chromosome* parent2,
                       Chromosome* child1, Chromosome* child2)
{
    // This method is empty in the original code and is left as such
}

void Ae::construct_distance_matrix(int dimension, vector<City>& cities) {
    distances = new int*[dimension];
    for(int i = 0; i < dimension; i++)
        distances[i] = new int[dimension];

    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            if (i == j) {
                distances[i][j] = 0;
            } else {
                distances[i][j] = static_cast<int>(euclidean_distance(cities[i], cities[j]) + 0.5); // Round to nearest integer
            }
        }
    }
}