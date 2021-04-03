#include <vector>
#include <set>
#include <iostream>
#include <cmath>


double getEuclideanDistance(std::vector<double> from, std::vector<double> to)
{
    double sum = 0;
    for (int i = 0; i < from.size(); i++)
    {
        sum += pow((to[i] - from[i]), 2);
    }
    return sqrt(sum);
}

double getRo(std::vector<double> current, std::vector<std::vector<double>> rest, int k)
{
    std::multiset<double, std::less<double>> euclideanSet;

    double sum = 0;
    double ro = 0;

    for (int i = 0; i < rest.size(); i++)
    {
        euclideanSet.insert(getEuclideanDistance(current, rest[i]));
    }

    std::multiset<double>::iterator it = euclideanSet.begin();
    for (int i = 0; i < k; ++i)
    {
        std::cout<< *it <<std::endl;
        sum += *it++;
    }

    if (k != 0)
    {
        return sum / k;
    }
    else
        return 0;
}
