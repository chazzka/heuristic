#include <vector>
using namespace std;

struct jedinec
{
	std::vector<double> position;
	double cost;
	double CR = 0.5;
	double F = 0.9;
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

vector<result> run(int dimension, int testFunction, int boundary);

inline vector<result> run(int dimension, int testFunction, int boundary) {

	int d = dimension;
	int P = 50; //teď už je to OK

	//helper vars
	int fezCounter = 0;
	int maxFez = 5000 * d;

	vector<result> bestResults;

	//generujeme populaci jedinců o velikosti P a dimenze D
	vector<jedinec> populace;
	for (int i = 0; i < P; i++) {

		vector<double> pozice;
		for (double prvek : generateRandom(d, -boundary, boundary)) {
			pozice.push_back(prvek);
		}
		jedinec tmpJedinec = { pozice, 0 };
		tmpJedinec.cost = 0;	
		cec20_test_func(pozice.data(), &tmpJedinec.cost, dimension, 1, testFunction);

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

			//předtím než vypočítám vi provedu mutování CR a F s P = 10%          
			double tau = 0.1;
			float Fl = 0.1;
			float Fu = 0.9;
			double rand1 = generateRandom(1, 0, 1).at(0);
			double rand2 = generateRandom(1, 0, 1).at(0);
			double rand3 = generateRandom(1, 0, 1).at(0);
			double rand4 = generateRandom(1, 0, 1).at(0);

			float potencialF = j.F;

			if (rand2 < tau) {
				potencialF = Fl + rand1 * Fu;
			}

			float potencialCR = j.CR;

			if (rand4 < tau) {
				potencialCR = rand3;
			}

			vector<double> vi;
			vector<double> ui;

			for (int i = 0; i < d; i++) {
				vi.push_back(dreiIdioten[0].position[i] + potencialF * (dreiIdioten[1].position[i] - dreiIdioten[2].position[i]));
			}
			//VRATIT DO SPRÁVNÝCH DIMENZÍ

			for (int ip = 0; ip < vi.size(); ip++) {
				if (-boundary > vi[ip] || vi[ip] > boundary) {
					vi[ip] = generateRandom(1, -boundary, boundary)[0];
				}
			}


			//generuj vektor ui, tak, že uděláš křížení vi a j
			int k = 0;
			for (double dimenze : vi) {
				double randomCR = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				int random0L = (rand() % d);
				if (randomCR < potencialCR || random0L == k) {
					ui.push_back(vi[k]);
				}
				else {
					ui.push_back(j.position[k]);
				}
				k++;
			}


			jedinec uiJedinec;
			uiJedinec.position = ui;
			uiJedinec.CR = potencialCR;
			uiJedinec.F = potencialF;
			double cenaUI = 0;
			cec20_test_func(uiJedinec.position.data(), &cenaUI, dimension, 1, testFunction);		
			uiJedinec.cost = cenaUI;

			if (uiJedinec.cost < j.cost) {				
				j.position = uiJedinec.position;
				j.cost = uiJedinec.cost;
				j.CR = uiJedinec.CR;
				j.F = uiJedinec.F;

				if (uiJedinec.cost < bestCost) {
					bestCost = uiJedinec.cost;
				}

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