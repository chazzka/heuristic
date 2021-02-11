#include <vector>
#include <iostream>
#include <random>

using namespace std;

struct jedinec
{
    int id;
    std::vector<double> position;
    double cost;
};

struct ohodnocenaPopulace
{
    jedinec leader;
    vector<jedinec> populace;
};

struct result
{
    int fez;
    double cost;
};


void cec20_test_func(double*, double*, int, int, int);

inline ohodnocenaPopulace getLeader(vector<jedinec> population, int testFunction, int dimension)
{
    //    double best_cost = first_dejong(population[0].position);
    double best_cost = 0;
    cec20_test_func(population[0].position.data(), &best_cost, dimension, 1, testFunction);

    jedinec tempLeader = population[0];
    //toto zlepšilo to, že leader nebude 0
    tempLeader.cost = best_cost;

    for (int i = 0; i < population.size(); i++) {
        //        double actual_cost = first_dejong(population[i].position);
        double actual_cost = 0;
        cec20_test_func(population[i].position.data(), &actual_cost, dimension, 1, testFunction);
        population[i].cost = actual_cost;
        if (actual_cost < best_cost) {
            best_cost = actual_cost;
            tempLeader = population[i];
        }
    }

    return { tempLeader, population };
}

inline vector<double> generateRandom(int size, double min, double max)
{
    std::vector<double> rndNumbers;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> distribution(min, max);

    //double randomNumber;
    for (int index = 0; index < size; index++) {
        double rand = distribution(gen);
        rndNumbers.push_back(rand);
    }

    return rndNumbers;
}

inline vector<double> vec_subtract(vector<double> first, vector<double> second) {
    vector<double> result;
    for (int i = 0; i < first.size(); i++) {
        result.push_back(first[i] - second[i]);
    }
    return result;
}

inline vector<double> vec_add(vector<double> first, vector<double> second) {
    vector<double> result;
    for (int i = 0; i < first.size(); i++) {
        result.push_back(first[i] + second[i]);
    }
    return result;
}

inline vector<double> vec_multiply_with_cislo(vector<double> first, double cislo) {
    vector<double> result;
    for (int i = 0; i < first.size(); i++) {
        result.push_back(first[i] * cislo);
    }
    return result;
}

inline vector<double> vec_multiply_with_vector(vector<double> first, vector<double> second) {
    vector<double> result;
    for (int i = 0; i < first.size(); i++) {
        result.push_back(first[i] * second[i]);
    }
    return result;
}

vector<result> run(int dimension, int testFunction, int boundary);


//nedojede do 50k, protože je omezeny migracemi - šenky říkal že to do konce nedojde
inline vector<result> run(int dimension, int testFunction, int boundary) {
    
    vector<result> best_results;
    double t = 0;
    int path_length = 3;
    double step = 0.33;
    double prt = 0.3;
    int d = dimension;
    int pop_size = 50; //dycky pade
    int pocet_accepted_fezu = 5000 * d;
    int migrace = 50;

    int konec = 0;
    int fezcounter = 0;
    //22050 fezů
    vector<jedinec> populace;
    for (int i = 0; i < pop_size; i++) {

        vector<double> pozice;
        for (double prvek : generateRandom(d, -boundary, boundary)) {
            pozice.push_back(prvek);
        }
        jedinec tmpJedinec = { i, pozice, 0 };
        populace.push_back(tmpJedinec);
    }

    ohodnocenaPopulace resPo = getLeader(populace, testFunction, dimension);
    jedinec leader = resPo.leader;
    populace = resPo.populace;

    //migrace
    
    for (int i_m = 0; i_m < migrace; i_m++) {
        jedinec leader_of_population = leader;       
        for (jedinec jedinec : populace) {
            if (konec == 1) {
                cout << "konec" << endl;
                continue;
            }
                                  
            if (jedinec.id != leader_of_population.id) {
                vector<double> potencial_position = jedinec.position;
                t = 0;
                double actual_cost = jedinec.cost;
                int countera = 0;
                //tady jsem to o 0.2 zmenšil aby to jelo jen 9x a ne 10x
                while (t < path_length-0.2) {
                    countera++;
                    
                    vector<double> PRTVector = generateRandom(dimension, 0, 1);

                    for (int i_p = 0; i_p < PRTVector.size(); i_p++) {
                        if (PRTVector[i_p] > prt) {
                            PRTVector[i_p] = 0;
                        }
                        else {
                            PRTVector[i_p] = 1;
                        }
                    }

                 
                    potencial_position = vec_add(potencial_position, vec_multiply_with_vector(vec_multiply_with_cislo(vec_subtract(leader.position, potencial_position), t), PRTVector));
                    //potencial position dobrý

                    //vrat do dimenzi - check OK
                    for (int ip = 0; ip < potencial_position.size(); ip++) {
                        if (-boundary > potencial_position[ip] || potencial_position[ip] > boundary) {
                            vector<double> randomNUm = generateRandom(1, -boundary, boundary);
                            potencial_position[ip] = randomNUm[0];
                        }
                    }

                    //zjisti zda se vylepsil                  
                    double potencial_cost = 0;
                    cec20_test_func(potencial_position.data(), &potencial_cost, dimension, 1, testFunction);

                    fezcounter++;                    
                    result res;
                    res.fez = fezcounter;
                    res.cost = leader.cost;
                    best_results.push_back(res);


                    if (fezcounter > pocet_accepted_fezu) {
                        konec = 1;
                        cout << "break fez";
                        break;
                    }


                    if (potencial_cost < actual_cost) {
                        if (potencial_cost < leader.cost) {
                            leader_of_population.position = potencial_position;
                            leader_of_population.cost = potencial_cost;
                        }
                        jedinec.cost = potencial_cost;
                        jedinec.position = potencial_position;
                    }
                    t += step;
                }
                //cout << "ahoj" << endl;
                //cout << countera << endl;

            }
            leader = leader_of_population;
        }

    }
   
    return best_results;
}
