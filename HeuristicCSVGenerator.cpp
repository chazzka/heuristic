#include <iostream>
#include <vector>
#include <time.h>
#include <string>
#include <fstream>



//CHOOSE ALGORITHM  (SOMA, JDE, PSO, PSO_NOV, DE)
#define PSO_NOV
//CHOOSE NUMBER OF RUNS
#define RUNS 10


#define BOUNDARY_LOW -100
#define BOUNDARY_UP 100

#ifdef SOMA
#include "alg/SOMA.cpp"
const char * alg_name = "SOMA";
#endif // SOMA

#ifdef DE
#include "alg/DE.cpp"
const char * alg_name = "DErand1bin";
#endif // DE

#ifdef JDE
#include "alg/JDE.cpp"
const char * alg_name = "JDE";
#endif // JDE

#ifdef PSO
#include "alg/pso.cpp"
const char * alg_name = "PSO";
#endif // PSO

#ifdef PSO_NOV
#include "alg/pso_nov1.cpp"
const char * alg_name = "PSO_NOV";
#endif // PSO_NOV


void makeCSVfile(std::string filename, std::vector<std::vector<result>> result)
{
	std::ofstream file;
	file.open("out/" + filename + ".csv");
	//for docker
	//file.open("/usr/src/results/" + filename + ".csv");
	std::vector<std::string> tempRes;
	for (unsigned int i = 0; i < result.size(); i++)
	{
		for (unsigned int p = 0; p < result.at(i).size(); p++)
		{

			if (i == 0)
			{
				tempRes.push_back(std::to_string(static_cast<int>(result.at(i).at(p).fez)) + ";" + std::to_string(result.at(i).at(p).cost));
			}
			else
			{
				tempRes.at(p) = tempRes.at(p) + ";" + std::to_string(result.at(i).at(p).cost);
			}
		}
	}
	for (int i = 0; i < tempRes.size(); i++)
	{
		file << tempRes.at(i) + "\n";
	}
	file.close();
}


int dimensionSize = 0;

int main()
{

	std::cout<< "start" << std::endl;
	
	srand((unsigned)time(0));

	
	//10 D
	dimensionSize = 10;
	std::vector<std::string> names = { "BendCigar", "RotatedSchwefel", "Lunacek", "Rosenbrock", "HybridOne", "HybridOneTwo", "HybridOneThree", "CompositionOne", "CompositionTwo", "CompositionThree" };
	for (int funkce = 1; funkce <= 10; funkce++) {
		std::vector<std::vector<result>> csv;
		for (int j = 0; j < RUNS; j++) {
			csv.push_back(run(dimensionSize, funkce, BOUNDARY_LOW, BOUNDARY_UP));
		}
		makeCSVfile(alg_name + names[funkce - 1] + std::to_string(dimensionSize) + "d", csv);
	}
	
	//20 D
	dimensionSize = 20;
	for (int funkce = 1; funkce <= 10; funkce++) {
		std::vector<std::vector<result>> csv;
		for (int j = 0; j < RUNS; j++) {
			csv.push_back(run(dimensionSize, funkce, BOUNDARY_LOW, BOUNDARY_UP));
		}
		makeCSVfile(alg_name + names[funkce - 1] + std::to_string(dimensionSize) + "d", csv);
	}
	
	std::cout<< "finish" << std::endl;
	

	return 0;
}


