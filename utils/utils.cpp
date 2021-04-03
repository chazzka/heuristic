#include <vector>
#include <set>
#include <cmath>

namespace utils
{
    namespace
    {
        double getEuclideanDistance(const std::vector<double>& from, const std::vector<double>& to)
        {
            double sum = 0;
            for (int i = 0; i < from.size(); i++)
            {
                sum += pow((to[i] - from[i]), 2);
            }
            return sqrt(sum);
        }
    }

    double getRo(const std::vector<double>& current, const std::vector<std::vector<double>>& all, int k)
    {
        std::multiset<double, std::less<double>> euclideanSet;

        double sum = 0;
        double ro = 0;

        for (int i = 0; i < all.size(); i++)
        {
            euclideanSet.insert(getEuclideanDistance(current, all[i]));
        }

        std::multiset<double>::iterator it = euclideanSet.begin();
        //begin from 1 - always ignore the first zero distance (self distance)
        for (int i = 1; i < k; ++i)
        {
            sum += *it++;
        }

        if (k != 0)
        {
            return sum / k;
        }
        else
            return 0;
    }

}
