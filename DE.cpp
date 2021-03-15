#include <vector>
#include <random>
#include <iostream>

struct jedinec
{
	std::vector<double> position;
	double cost;
};

struct result
{
	int fez;
	double cost;
};

std::vector<double> generateRandom(int size, double min, double max)
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

std::vector<jedinec> get3unique(std::vector<jedinec> populace, jedinec jed)
{

	int i = 0;
	for (jedinec j : populace)
	{
		if (j.position == jed.position)
		{
			populace.erase(populace.begin() + i);
			break;
		}
		i++;
	}

	std::vector<jedinec> result;

	for (int i = 0; i < 3; i++)
	{
		int ran = (rand() % populace.size());
		result.push_back(populace.at(ran));
		populace.erase(populace.begin() + ran);
	}

	return result;
}

void cec20_test_func(double *, double *, int, int, int);

//DE/rand/1/bin
std::vector<result> run(int dimension, int testFunction, double boundaryLow, double boundaryUp)
{
	//Choose the parameters
	int d = dimension;
	int P = 50;
	float CR = 0.8;
	double F = 0.5;
	int iterations = 1000;

	//helper vars
	int fezCounter = 0;
	int maxFez = 5000 * d;

	std::vector<result> bestResults;

	//Initialize all agents x with random positions in the search-space.
	std::vector<jedinec> populace;
	std::vector<double> gPosition = generateRandom(d, boundaryLow, boundaryUp);
	double gCost = 0;
	cec20_test_func(gPosition.data(), &gCost, dimension, 1, testFunction);

	for (int i = 0; i < P; i++)
	{
		std::vector<double> pozice;
		for (double xd : generateRandom(d, boundaryLow, boundaryUp))
		{
			pozice.push_back(xd);
		}
		jedinec tmpJedinec = {pozice, 0};
		tmpJedinec.cost = 0;
		cec20_test_func(pozice.data(), &tmpJedinec.cost, dimension, 1, testFunction);
		populace.push_back(tmpJedinec);
		if (tmpJedinec.cost < gCost)
		{
			gPosition = tmpJedinec.position;
			gCost = tmpJedinec.cost;
		}
	}

	while (iterations--)
	{
		std::vector<jedinec> uiPopulace;
		//For each agent x in the population do:
		for (jedinec x : populace)
		{
			//Pick three unique agents
			std::vector<jedinec> threeUnique = get3unique(populace, x);
			//Pick a random index R
			double R = generateRandomDouble(1, d);
			//Compute the agent's potentially new position y
			std::vector<double> y;
			for (int i = 0; i < d; i++)
			{
				//pick a uniformly distributed random number ri
				double r = generateRandomDouble(0, 1);
				if (r < CR || i == R)
				{
					double yi = threeUnique[0].position[i] + F * (threeUnique[1].position[i] - threeUnique[2].position[i]);
					//out of dimension check
					if (boundaryLow > yi || yi > boundaryUp)
					{
						y.push_back(generateRandomDouble(boundaryLow, boundaryUp));
					}
					else
					{
						y.push_back(yi);
					}
				}
				// otherwise set yi = xi
				else
				{
					y.push_back(x.position[i]);
				}
			}
			double yCost = 0;
			cec20_test_func(y.data(), &yCost, dimension, 1, testFunction);
			fezCounter++;

			result res;
			res.cost = gCost;
			res.fez = fezCounter;
			bestResults.push_back(res);

			if (fezCounter > maxFez)
            {
                std::cout << "break fez";
                return bestResults;
            }

			if (yCost <= x.cost)
			{
				//then replace the agent x in the population with  y.
				x.position = y;
				x.cost = yCost;
				if (yCost < gCost)
				{
					gCost = yCost;
					gPosition = y;
				}
			}
		}
	}
	return bestResults;
}
