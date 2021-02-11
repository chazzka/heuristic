#include <iostream>
#include <vector>
#include <time.h>
#include <string>
#include <fstream>
#include "cec20_test_func.cpp"

//ZDE SI VYBER FUNCKI  (SOMA, JDE)
#define SOMA
//zbytek se děje automaticky, můžeš jít spát


#ifdef SOMA
#include "SOMA.cpp"
#endif // SOMA

#ifdef JDE
#include "JDE.cpp"
#endif // JDE


using namespace std;

#define BOUNDARY 100

void cec20_test_func(double*, double*, int, int, int);


void makeCSVfile(string filename, std::vector<std::vector<result>> result)
{
	ofstream file;
	file.open("/usr/src/results/" + filename + ".csv");
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
const int runs = 1;

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
			csv.push_back(run(dimensionSize, funkce, BOUNDARY));
		}
#ifdef SOMA
		makeCSVfile("SOMA" + names[funkce - 1] + to_string(dimensionSize) + "d", csv);
#endif // SOMA
#ifdef JDE
		makeCSVfile("JDE" + names[funkce - 1] + to_string(dimensionSize) + "d", csv);
#endif // JDE


	}

	//20 D
	dimensionSize = 20;
	for (int funkce = 1; funkce <= 10; funkce++) {
		std::vector<std::vector<result>> csv;
		for (int j = 0; j < runs; j++) {
			csv.push_back(run(dimensionSize, funkce, BOUNDARY));
		}
#ifdef SOMA
		makeCSVfile("SOMA" + names[funkce - 1] + to_string(dimensionSize) + "d", csv);
#endif // SOMA
#ifdef JDE
		makeCSVfile("JDE" + names[funkce - 1] + to_string(dimensionSize) + "d", csv);
#endif // JDE


	}

	cout<< "finish" << endl;

	return 0;
}


