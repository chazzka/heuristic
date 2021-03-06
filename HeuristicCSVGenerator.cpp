﻿#include <iostream>
#include <vector>
#include <time.h>
#include <string>
#include <fstream>
#include "cec20_test_func.cpp"

//ZDE SI VYBER FUNCKI  (SOMA, JDE, PSO, DE)
#define DE
//zbytek se děje automaticky, můžeš jít spát


#ifdef SOMA
#include "SOMA.cpp"
const char * alg_name = "SOMA";
#endif // SOMA

#ifdef DE
#include "DE.cpp"
const char * alg_name = "DErand1bin";
#endif // SOMA

#ifdef JDE
#include "JDE.cpp"
const char * alg_name = "JDE";
#endif // JDE

#ifdef PSO
#include "pso.cpp"
const char * alg_name = "PSO";
#endif // PSO


using namespace std;

#define BOUNDARY_LOW -100
#define BOUNDARY_UP 100

void cec20_test_func(double*, double*, int, int, int);


void makeCSVfile(string filename, std::vector<std::vector<result>> result)
{
	ofstream file;
	//for docker
	//file.open("/usr/src/results/" + filename + ".csv");
	file.open(filename + ".csv");
	std::vector<string> tempRes;

	for (unsigned int i = 0; i < result.size(); i++)
	{
		for (unsigned int p = 0; p < result.at(i).size(); p++)
		{

			if (i == 0)
			{
				tempRes.push_back(to_string(static_cast<int>(result.at(i).at(p).fez)) + ";" + to_string(result.at(i).at(p).cost));
			}
			else
			{
				tempRes.at(p) = tempRes.at(p) + ";" + to_string(result.at(i).at(p).cost);
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
const int runs = 10;

int main()
{

	cout<< "start" << endl;
	
	srand((unsigned)time(0));


	//10 D
	dimensionSize = 10;
	std::vector<string> names = { "BendCigar", "RotatedSchwefel", "Lunacek", "Rosenbrock", "HybridOne", "HybridOneTwo", "HybridOneThree", "CompositionOne", "CompositionTwo", "CompositionThree" };
	for (int funkce = 1; funkce <= 10; funkce++) {
		std::vector<std::vector<result>> csv;
		for (int j = 0; j < runs; j++) {
			csv.push_back(run(dimensionSize, funkce, BOUNDARY_LOW, BOUNDARY_UP));
		}
		makeCSVfile(alg_name + names[funkce - 1] + to_string(dimensionSize) + "d", csv);
	}

	//20 D
	dimensionSize = 20;
	for (int funkce = 1; funkce <= 10; funkce++) {
		std::vector<std::vector<result>> csv;
		for (int j = 0; j < runs; j++) {
			csv.push_back(run(dimensionSize, funkce, BOUNDARY_LOW, BOUNDARY_UP));
		}
		makeCSVfile(alg_name + names[funkce - 1] + to_string(dimensionSize) + "d", csv);
	}

	cout<< "finish" << endl;
	

	return 0;
}


