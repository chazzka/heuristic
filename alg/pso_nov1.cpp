#include <vector>
#include <iostream>
#include <random>
#include "../cec20_test_func.cpp"
#include "../utils/utils.cpp"

#define K 10

struct result
{
    int fez;
    double cost;
};

struct Particle
{
    std::vector<double> positionXi;
    std::vector<double> velocityVectorVi;
    std::vector<double> pBestPi;
    double pBestCost;
    double ro;
};

std::vector<double> generateRandomRange(int size, double min, double max)
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

double generateRandomDouble(double min, double max)
{

    return (max - min) * ((double)rand() / (double)RAND_MAX) + min;
}

std::vector<result> run(int dimension, int testFunction, int boundaryLow, int boundaryUp)
{
    //vars
    int MAX_FEZ = 5000 * dimension;
    std::vector<result> best_results;
    //Engelbrecht
    int generations = 5000;
    int popSize = 30;
    double c1 = 1.496180;
    double c2 = c1;
    double w = 0.729844;
    double learningRate = 0.8;
    int fezCounter = 0;

    std::vector<Particle> population;
    //leader
    std::vector<double> leadingPosG = generateRandomRange(dimension, boundaryLow, boundaryUp);
    double gCost = 0;
    cec20_test_func(leadingPosG.data(), &gCost, dimension, 1, testFunction);

    //positions will be used for novelty evaluation
    std::vector<std::vector<double>> positions;
    //create particles and evaluate cost functions
    for (int i = 0; i < popSize; i++)
    {
        std::vector<double> randomPosition = generateRandomRange(dimension, boundaryLow, boundaryUp);
        positions.push_back(randomPosition);
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

    //evaluate novelty and get most unique one
    double biggestRo = 0; //biggestRo is most unique
    Particle mostUnique;
    for (int i = 0; i < popSize; i++)
    {
        population[i].ro = utils::getRo(population[i].positionXi, positions, K);
        if (population[i].ro > biggestRo)
        {
            biggestRo = population[i].ro;
            mostUnique = population[i];
        }
    }

    for (int g = 0; g < generations; g++)
    {
        for (int i = 0; i < population.size(); i++)
        {

            //positions are being changed during this cycle, keep track of the most unique one
            double currentBestRo = utils::getRo(population[i].positionXi, positions, K);
            if(currentBestRo > biggestRo)
            {
                mostUnique = population[i];
            }

            for (int d = 0; d < dimension; d++)
            {
                double rp = generateRandomDouble(0, 1);
                double rg = generateRandomDouble(0, 1);

                //get some random to go for novelty only in 1:X
                double noveltyRandom = generateRandomDouble(0, 1);
                int novelty = 0;
                //Update the particle's velocity
                if (noveltyRandom > 0.2) // do classic
                {
                    population[i].velocityVectorVi[d] = w * population[i].velocityVectorVi[d] + c1 * rp * (population[i].pBestPi[d] - population[i].positionXi[d]) + c2 * rg * (leadingPosG[d] - population[i].positionXi[d]);
                }
                else //do novelty
                {
                    population[i].velocityVectorVi[d] = w * population[i].velocityVectorVi[d] + (mostUnique.positionXi[d] - population[i].positionXi[d]);
                }

                //Update the particle's position:  můžu ve stejnem foru?
                population[i].positionXi[d] = population[i].positionXi[d] + learningRate * population[i].velocityVectorVi[d];

                //boundary check
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
                std::cout << "break fez" << std::endl;
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