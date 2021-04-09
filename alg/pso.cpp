#include <vector>
#include <iostream>
#include <random>
#include "../cec20_test_func.cpp"

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
    int popSize = 25; //nebo 50, hlavně ať je to dělitelné 5000 - experimentuj a dívej se jak to vychází
    int generations = MAX_FEZ / popSize;
    double c1 = 1.496180; //Engelbrecth
    double c2 = c1;
    double wStart = 0.9;
    double wEnd = 0.4;
    double vMax = 0.2;
    double w = wStart;
    int fezCounter = 0;

    std::vector<Particle> population;
    //leader
    std::vector<double> leadingPosG = generateRandomRange(dimension, boundaryLow, boundaryUp);
    double gCost = 0;
    cec20_test_func(leadingPosG.data(), &gCost, dimension, 1, testFunction);

    for (int i = 0; i < popSize; i++)
    {
        std::vector<double> randomPosition = generateRandomRange(dimension, boundaryLow, boundaryUp);
        //Initialize the particle's position with a uniformly distributed random vector xi ~ U(blo, bup)
        //TODO initial velocity 0?
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
                double potentialVectorD = w * population[i].velocityVectorVi[d] + c1 * rp * (population[i].pBestPi[d] - population[i].positionXi[d]) + c2 * rg * (leadingPosG[d] - population[i].positionXi[d]);

                //TODO: zkontrolovat
                //musíš zkontrolovat jestli ten nový velocity vector není větší než 0.2*200 (vMax * |-100-100|) - pozor bounds je vektor, každá dimenze může mít jiný rozsah, bound[d]
                //pozor velikost vektoru může být negativní (takže musíš kontrolvat i vektor co míří na druhou stranu - mínusový musí pak ale mířit zas do mínusu!)
                //pozor, výsledek může být 40 ale i -40
                if (abs(potentialVectorD) > vMax * (boundaryUp - boundaryLow))
                {
                    if (potentialVectorD < 0)
                    {
                        population[i].velocityVectorVi[d] = -potentialVectorD;
                    }
                    else
                    {
                        population[i].velocityVectorVi[d] = potentialVectorD;
                    }
                }
                else
                {
                    if (potentialVectorD < 0)
                    {
                        population[i].velocityVectorVi[d] = -vMax;
                    }
                    else
                    {
                        population[i].velocityVectorVi[d] = vMax;
                    }
                }

                //Update the particle's position
                population[i].positionXi[d] = population[i].positionXi[d] + population[i].velocityVectorVi[d];

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

            result res;
            res.fez = fezCounter;
            res.cost = gCost;
            best_results.push_back(res);
        }
        //after generation run, recount w
        w = wStart - ((wStart - wEnd) * g) / generations;
    }

    return best_results;
}