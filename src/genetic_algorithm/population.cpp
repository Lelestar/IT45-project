#include "population.h"

using namespace std;

// initialization of a population of solutions
Population::Population(int ps, int cs)
{
    population_size = ps;
    individuals = new Chromosome*[population_size];
    for (int i = 0; i < population_size; i++)
        individuals[i] = new Chromosome(cs);
    order = new int[population_size];
}

// destruction of the "Population" object
Population::~Population()
{
    for (int i = 0; i < population_size; i++)
        delete individuals[i];
    delete[] individuals;
    delete[] order;
}

// statistics about the population
void Population::statistics()
{
    double mean = 0;
    double standard_deviation = 0;

    for (int i = 0; i < population_size; i++)
    {
        mean += individuals[i]->fitness;
        standard_deviation += individuals[i]->fitness * individuals[i]->fitness;
    }
    mean = mean / population_size;
    standard_deviation = sqrt(standard_deviation / population_size - mean * mean);

    cout << "fitness : (mean, standard deviation) -> ("
         << mean << " , " << standard_deviation << ")" << endl;
    cout << "fitness : [best, median, worst] -> ["
         << individuals[order[0]]->fitness << " , "
         << individuals[order[(int)(population_size / 2)]]->fitness << " , "
         << individuals[order[population_size - 1]]->fitness << "]" << endl;
}

// Similarity of the population
void Population::similarity()
{
    int num_ind_id_1, num_ind_id_2, num_ind_id_3;
    num_ind_id_1 = num_similar_chromosomes(individuals[order[0]]);
    cout << "Number of identical individuals in the population with fitness = " << individuals[order[0]]->fitness << " : " << num_ind_id_1 << " / " << population_size << endl;
    if (num_ind_id_1 < population_size)
    {
        num_ind_id_2 = num_similar_chromosomes(individuals[order[num_ind_id_1]]);
        cout << "Number of identical individuals in the population with fitness = " << individuals[order[num_ind_id_1]]->fitness << " : " << num_ind_id_2 << " / " << population_size << endl;
        if (num_ind_id_1 + num_ind_id_2 < population_size)
        {
            num_ind_id_3 = num_similar_chromosomes(individuals[order[num_ind_id_1 + num_ind_id_2]]);
            cout << "Number of identical individuals in the population with fitness = " << individuals[order[num_ind_id_1 + num_ind_id_2]]->fitness << " : " << num_ind_id_3 << " / " << population_size << endl;
        }
    }
}

// counts the number of similar chromosomes
int Population::num_similar_chromosomes(Chromosome* chrom)
{
    int num = 0;
    for (int i = 0; i < population_size; i++)
        if (chrom->identical(individuals[i]))
            num++;
    return num;
}

// Sorts the individuals in the population in ascending order of fitness
void Population::sort()
{
    int temp;
    for (int i = 0; i < population_size; i++)
        order[i] = i;

    for (int i = 0; i < population_size - 1; i++)
        for (int j = i + 1; j < population_size; j++)
            if (individuals[order[i]]->fitness > individuals[order[j]]->fitness)
            {
                temp = order[i];
                order[i] = order[j];
                order[j] = temp;
            }
}

// Re-sorts the ranking of individuals in the population in ascending order of fitness
//   after a small change
void Population::resort()
{
    int temp;
    for (int i = 0; i < population_size - 1; i++)
        for (int j = i + 1; j < population_size; j++)
            if (individuals[order[i]]->fitness > individuals[order[j]]->fitness)
            {
                temp = order[i];
                order[i] = order[j];
                order[j] = temp;
            }
}

// BIASED ROULETTE SELECTION
// selection operator based on the fitness function
Chromosome* Population::roulette_selection()
{
    int fitness_sum = individuals[0]->fitness;
    int fitness_max = individuals[0]->fitness;
    int portion_sum;

    for (int i = 1; i < population_size; i++)
    {
        fitness_sum += individuals[i]->fitness;
        if (fitness_max < individuals[i]->fitness)
            fitness_max = individuals[i]->fitness;
    }
    portion_sum = fitness_max * population_size - fitness_sum;

    double random_variable = Random::random(1000) / 1000.0;

    int ind = 0;
    double portion = (fitness_max - individuals[0]->fitness) * 1.0 / portion_sum;
    while ((ind < population_size - 1) && (random_variable >= portion))
    {
        ind++;
        portion += (fitness_max - individuals[ind]->fitness) * 1.0 / portion_sum;
    }
    return individuals[ind];
}

// biased roulette replacement operator for an individual in the population
//   with a new given individual
void Population::roulette_replacement(Chromosome* individual)
{
    int fitness_sum = individuals[0]->fitness;
    for (int i = 1; i < population_size; i++)
        fitness_sum += individuals[i]->fitness;

    double random_variable;
    int ind = order[0];
    double portion;

    while (order[0] == ind)
    {
        random_variable = Random::random(1000) / 1000.0;
        ind = 0;
        portion = individuals[0]->fitness * 1.0 / fitness_sum;
        while ((ind < population_size - 1) && (random_variable > portion))
        {
            ind++;
            portion += individuals[ind]->fitness * 1.0 / fitness_sum;
        }
    }
    individuals[ind]->copy(individual);
    individuals[ind]->fitness = individual->fitness;
}

// RANDOM SELECTION
// random selection operator
Chromosome* Population::random_selection()
{
    int random_ind = Random::random(population_size);
    return individuals[random_ind];
}

// random replacement operator for an individual in the population
//   with a new given individual
void Population::random_replacement(Chromosome* individual)
{
    int random_ind = Random::random(population_size);
    individuals[random_ind]->copy(individual);
    individuals[random_ind]->fitness = individual->fitness;
}

// RANKING SELECTION
// selection operator based on ranking, the individuals in the
//   population are ordered so that the best is rank 0
//   the ranking rate adjusts the selection pressure:
//   0 strong pressure and +INFINITY weak pressure (probability = 1/nb regardless of the individual)
Chromosome* Population::ranking_selection(float ranking_rate)
{
    resort();

    double random_variable = Random::random(10000) / 10000.0;
    int num = population_size;
    int i = 0;
    ranking_rate = ranking_rate / 100;

    double portion = ((double(num - i) * 2.0 / double(num * (num + 1))) + ranking_rate) / (1.0 + num * ranking_rate);

    while (random_variable > portion)
    {
        i++;
        portion += ((double(num - i) * 2.0 / double(num * (num + 1))) + ranking_rate) / (1.0 + num * ranking_rate);
    }

    if (i >= num)
        i = Random::random(num);
    return individuals[order[i]];
}

// ranking replacement operator for an individual in the population
//   with a new given individual
void Population::ranking_replacement(Chromosome* individual, float ranking_rate)
{
    double random_variable = Random::random(10000) / 10000.0;
    int T = population_size;
    int i = 0;
    ranking_rate = ranking_rate / 100;
    double portion = ((double(i) * 2.0 / double(T * (T - 1))) + ranking_rate) / (1.0 + T * ranking_rate);

    while (random_variable > portion)
    {
        i++;
        portion += ((double(i) * 2.0 / double(T * (T - 1))) + ranking_rate) / (1.0 + T * ranking_rate);
    }
    if (i >= T)
        i = Random::random(T);
    int ind = order[i];
    individuals[ind]->copy(individual);
    individuals[ind]->fitness = individual->fitness;
}

// displays the population, its rank, and its fitness
void Population::display()
{
    cout << "Population of " << population_size << " individuals :" << endl;
    for (int i = 0; i < population_size; i++)
    {
        cout << "individual " << i << ", rank: " << order[i] << " ";
        individuals[i]->display();
    }
}
