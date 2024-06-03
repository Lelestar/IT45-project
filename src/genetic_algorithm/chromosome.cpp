#include "chromosome.h"

using namespace std;

// initialization of the parameters of a chromosome
Chromosome::Chromosome(int cs)
{
    int a;
    bool retry = true;
    size = cs;
    // a chromosome is composed of 'size' genes,
    // genes are characterized by an integer between 0 and 'size-1'
    // it cannot have the same gene twice in a chromosome
    genes = new int[size];
    // Arbitrarily, we choose to always start a chromosome with gene 0
    genes[0] = 0;
    for(int i = 1; i < size; i++)
    {
        retry = true;
        while(retry)
        {
            retry = false;
            // randomly pick the next gene
            a = Random::random(size);
            // the gene must not already be in the chromosome
            // if it is, we randomly pick the gene again
            for (int j = 0; j < i; j++)
                if (a == genes[j])
                    retry = true;
        }
        genes[i] = a;
    }
    // we arbitrarily impose that gene[1] > gene[size-1]
    sort();
}

// destruction of the 'Chromosome' object
Chromosome::~Chromosome()
{
    delete[] genes;
}

// we arbitrarily impose that the 2nd visited city (gene[1])
//   has a number smaller than the last visited city (gene[size-1])
//   i.e. : gene[1] > gene[size-1]
void Chromosome::sort()
{
    int temp, k;
    // Place city "0" at the head of the chromosome (genes[0])
    if (genes[0] != 0)
    {
        int position_0 = 0;
        int * genes_c = new int[size];
        for (int i = 0; i < size; i++)
        {
            genes_c[i] = genes[i];
            if (genes[i] == 0)
                position_0 = i;
        }
        k = 0;
        for (int i = position_0; i < size; i++)
        {
            genes[k] = genes_c[i];
            k++;
        }
        for (int i = 0; i < position_0; i++)
        {
            genes[k] = genes_c[i];
            k++;
        }
        delete[] genes_c;
    }

    // The number of the 2nd city must be smaller than that of the last city
    if (genes[1] > genes[size - 1])
    {
        for(int k = 1; k <= 1 + (size - 2) / 2; k++)
        {
            temp = genes[k];
            genes[k] = genes[size - k];
            genes[size - k] = temp;
        }
    }
}

// evaluation of a solution: function that calculates the fitness of a solution
void Chromosome::evaluate(int **distance)
{
    fitness = 0;
    for(int i = 0; i < size - 1; i++)
        fitness += distance[genes[i]][genes[i + 1]];
    fitness += distance[genes[0]][genes[size - 1]];
}

// copies the genes of a chromosome. fitness is not copied
void Chromosome::copy(Chromosome* source)
{
    for(int i = 0; i < size; i++)
        genes[i] = source->genes[i];
}

// swaps 2 genes
void Chromosome::swap_2_genes(int gene1, int gene2)
{
    int temp = genes[gene1];
    genes[gene1] = genes[gene2];
    genes[gene2] = temp;
}

void Chromosome::swap_2_consecutive_genes()
{
    // randomly select a gene between the first and the second to last.
    // Reminder: Random::random(size - 1) returns a random integer between 0 and size - 2
    int i = Random::random(size - 1);

    // swap the randomly selected gene with the succeeding gene
    swap_2_genes(i, i + 1);

    sort();
}

void Chromosome::swap_2_random_genes()
{
    // randomly select 2 genes
    int i = Random::random(size);
    int j = Random::random(size);

    // swap the 2 genes
    swap_2_genes(i, j);

    sort();
}

void Chromosome::move_1_gene()
{
    // This method is empty in the original code and is left as such
}

void Chromosome::invert_sequence_of_genes()
{
    // randomly select two cut points
    int i = Random::random(size);
    int j = Random::random(size);

    // invert the sequence of genes between the two cut points
    while(i < j)
    {
        swap_2_genes(i, j);
        i++;
        j--;
    }

    sort();
}

// display the parameters of a chromosome
void Chromosome::display()
{
    cout << genes[0];
    for(int i = 1; i < size; i++)
        cout << "-" << genes[i];
    cout << " => fitness = " << fitness << endl;
}

bool Chromosome::identical(Chromosome* chrom)
{
    for(int i = 1; i < size; i++)
        if (chrom->genes[i] != this->genes[i])
            return false;
    return true;
}
