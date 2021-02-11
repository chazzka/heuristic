#include <vector>
using namespace std;

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

inline vector<double> generateRandom(int size, double min, double max)
{
	std::vector<double> rndNumbers;


	double randomNumber;
	for (int index = 0; index < size; index++) {

		randomNumber = (max - min) * ((double)rand() / (double)RAND_MAX) + min;
		rndNumbers.push_back(randomNumber);
	}

	return rndNumbers;
}

inline vector<jedinec> get3blbecky(vector<jedinec> populace, jedinec jed) {

	int i = 0;
	for (jedinec j : populace) {
		if (j.position == jed.position) {
			populace.erase(populace.begin() + i);
			break;
		}
		i++;
	}

	vector<jedinec> result;

	for (int i = 0; i < 3; i++) {
		int ran = (rand() % populace.size());
		result.push_back(populace.at(ran));
		populace.erase(populace.begin() + ran);
	}

	return result;
}

void cec20_test_func(double*, double*, int, int, int);

vector<result> run(int dimension, int testFunction);

//TODO: dodělat dimension a seed a boundary !!!!!
inline vector<result> run(int dimension, int testFunction) {
	//inputs
	int d = dimension;
	int P = 10 * d;
	float c = 0.5;
	double F = 0.9;

	//helper vars
	int fezCounter = 0;
	int maxFez = 5000 * d;

	vector<result> bestResults;

	//generujeme populaci jedinců o velikosti P a dimenze D
	vector<jedinec> populace;
	for (int i = 0; i < P; i++) {

		vector<double> pozice;
		for (double prvek : generateRandom(d, -100, 100)) {
			pozice.push_back(prvek);
		}
		jedinec tmpJedinec = { pozice, 0 };
		tmpJedinec.cost = 0;
		cec20_test_func(pozice.data(), &tmpJedinec.cost, 10, 1, testFunction);
		populace.push_back(tmpJedinec);
	}


	double currentCost = populace[0].cost;
	double bestCost = currentCost;

	while (fezCounter <= maxFez - P) {

		vector<jedinec> uiPopulace;
		for (jedinec j : populace) {
			//pro kazdeho jedince uděláme mutační trik dle strategie RAND
			//VYBER 3 NÁHODNÉ BODY z populace které se od sebe liší a liší se i od právě probíraného jedince
			vector<jedinec> tempPopul = populace;
			vector<jedinec> dreiIdioten = get3blbecky(tempPopul, j);
			vector<double> vi;
			vector<double> ui;

			for (int i = 0; i < d; i++) {
				vi.push_back(dreiIdioten[0].position[i] + F * (dreiIdioten[1].position[i] - dreiIdioten[2].position[i]));
			}
			//VRATIT DO SPRÁVNÝCH DIMENZÍ

			for (int ip = 0; ip < vi.size(); ip++) {
				if (-5.12 > vi[ip] || vi[ip] > 5.12) {
					vi[ip] = generateRandom(1, -5.12, 5.12).at(0);
				}
			}


			//generuj vektor ui, tak, že uděláš křížení vi a j
			int k = 0;
			for (double dimenze : vi) {
				double randomCR = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				int random0L = (rand() % d);
				if (randomCR < c || random0L == k) {
					ui.push_back(vi[k]);
				}
				else {
					ui.push_back(j.position[k]);
				}
				k++;
			}

			jedinec uiJedinec;
			uiJedinec.position = ui;
			uiPopulace.push_back(uiJedinec);

		}

		for (int j = 0; j < P; j++) {
			double cenaUI = 0;
			cec20_test_func(uiPopulace[j].position.data(), &cenaUI, 10, 1, testFunction);
			double cenaXI = populace[j].cost;


			currentCost = cenaXI;
			if (cenaUI < cenaXI) {
				populace[j].position = uiPopulace[j].position;
				populace[j].cost = cenaUI;
				currentCost = cenaUI;
			}
			if (currentCost < bestCost) {
				bestCost = currentCost;
			}

			fezCounter++;
			result res;
			res.fez = fezCounter;
			res.cost = bestCost;
			bestResults.push_back(res);

		}
	}
	return bestResults;
}


