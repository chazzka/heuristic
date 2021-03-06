#include <vector>
#include <iostream>
#include <random>

using namespace std;

struct result
{
    int fez;
    double cost;
};

struct Particle
{
    vector<double> positionXi;
    vector<double> velocityVectorVi;
    vector<double> pBestPi; //best known position
    double pBestCost;
};

inline vector<double> generateRandomRange(int size, double min, double max)
{
    std::vector<double> rndNumbers;

    double randomNumber;
    for (int index = 0; index < size; index++)
    {

        randomNumber = (max - min) * ((double)rand() / (double)RAND_MAX) + min;
        rndNumbers.push_back(randomNumber);
    }

    return rndNumbers;
}

inline double generateRandomDouble(double min, double max)
{

    return (max - min) * ((double)rand() / (double)RAND_MAX) + min;
}

void cec20_test_func(double *, double *, int, int, int);

vector<result> run(int dimension, int testFunction, int boundaryLow, int boundaryUp);

inline vector<result> run(int dimension, int testFunction, int boundaryLow, int boundaryUp)
{
    //vars
    int MAX_FEZ = 5000 * dimension;
    vector<result> best_results;
    int generations = 10;
    int popSize = 10;
    int c1 = 2;
    int c2 = 2;
    double w = 0.7; //TODO: DO not know yet
    double learningRate = 0.8;
    int fezCounter = 0;

    vector<Particle> population;
    //leader
    vector<double> leadingPosG = generateRandomRange(dimension, boundaryLow, boundaryUp);
    double gCost = 0;
    cec20_test_func(leadingPosG.data(), &gCost, dimension, 1, testFunction);

    for (int i = 0; i < popSize; i++)
    {
        vector<double> randomPosition = generateRandomRange(dimension, boundaryLow, boundaryUp);
        //Initialize the particle's position with a uniformly distributed random vector xi ~ U(blo, bup)
        //Initialize the particle's velocity: vi ~ U(-|bup-blo|, |bup-blo|)
        //Initialize the particle's best known position to its initial position: pi ← xi
        Particle p = {randomPosition, generateRandomRange(dimension, -(boundaryUp - boundaryLow), (boundaryUp - boundaryLow)), randomPosition};
        double pCost = 0;
        cec20_test_func(p.pBestPi.data(), &pCost, dimension, 1, testFunction);
        //fezCounter++;
        //ulozim si tady jeste cost a pak dole nemusim delat dalsi evaluaci
        p.pBestCost = pCost;
        //find best swarm position and cost
        if (pCost < gCost)
        {
            //update the swarm's best known position: g ← pi
            leadingPosG = p.pBestPi;
            gCost = pCost;
        }
        population.push_back(p);
    }

    for (int g = 0; g < generations; g++)
    {
        for (int i = 0; i < population.size(); i++)
        {
            for (int d = 0; d < dimension; d++)
            {
                double rp = generateRandomDouble(0, 1);
                double rg = generateRandomDouble(0, 1);
                //Update the particle's velocity
                population[i].velocityVectorVi[d] = w * population[i].velocityVectorVi[d] + c1 * rp * (population[i].pBestPi[d] - population[i].positionXi[d]) + c2 * rg * (leadingPosG[d] - population[i].positionXi[d]);
                //Update the particle's position:  můžu ve stejnem foru?
                population[i].positionXi[d] = population[i].positionXi[d] + learningRate * population[i].velocityVectorVi[d];                
                if (boundaryLow > population[i].positionXi[d] || population[i].positionXi[d] > boundaryUp)
                {
                    double randomNum = generateRandomDouble((double)boundaryLow, (double)boundaryUp);
                    population[i].positionXi[d] = randomNum;
                }
            }
            double newXiCost = 0;
            cec20_test_func(population[i].positionXi.data(), &newXiCost, dimension, 1, testFunction);
            fezCounter++;

            result res;
            res.fez = fezCounter;
            res.cost = gCost;
            best_results.push_back(res);

            if (fezCounter > MAX_FEZ)
            {
                cout << "break fez";
                return best_results;
            }

            if (newXiCost < population[i].pBestCost)
            {
                //Update the particle's best known position
                population[i].pBestPi = population[i].positionXi;
                population[i].pBestCost = newXiCost;
                if (newXiCost < gCost)
                {
                    //Update the swarm's best known position
                    gCost = newXiCost;
                    leadingPosG = population[i].positionXi;
                }
            }
        }
    }

    return best_results;
}