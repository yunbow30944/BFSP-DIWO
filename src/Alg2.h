//
// Created by DELL on 24-11-20.
//

#ifndef ALG2_H
#define ALG2_H
#include <random>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <vector>
using namespace std;

namespace ALG2 {
    extern int t_max;

    const double epsilon = 1e-9;

    class Sigma {
    private:
        clock_t start; //算法开始时间用start表示

    public:
        vector<double> sigma_i_k;
        double sigma_k;
        vector<vector<int> > pop;

        Sigma() {
            start = clock();
        }

        Sigma(const vector<vector<int> > &pop) : pop(pop) {
            start = clock();
        }

        ~Sigma() = default;

        void calculate_sigma_i_k();

        double calculate_sigma_k(double sigma_min, double sigma_max) const ;

        void resetStart() {
            start = clock();
        }

        void resetPOP(const vector<vector<int>> &POP) {
            this->pop.clear();
            for (const auto &i: POP)
                this->pop.push_back(i);
        }

        inline int getRunTime() const {
            return (clock()-this->start)/CLOCKS_PER_SEC*1000;
        }
    };
    int generate_d(Sigma &sigma, int i, const double sigma_min, const double sigma_max);
}

vector<int> reproduction(int S_min, int S_max);

vector<vector<int> > spatialDispersal(vector<int> s, double sigma_min, double sigma_max, ALG2::Sigma &sig);
#endif //ALG2_H
