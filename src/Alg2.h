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
    // 单位：毫秒
    // const int rho = 30;
    // const int rho = 60;
    const int rho = 90;
    extern int t_max;

    const double epsilon = 1e-9;
    const vector<int> S_MIN{0, 1, 2};
    const vector<int> S_MAX{5, 7, 9};
    const vector<int> SIGMA_MIN{3, 4, 5};
    const vector<int> SIGMA_MAX{8, 9, 10};

    class Sigma {
    private:
        clock_t start; //此处CPU最大执行时间与算法开始时间均用start表示

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

        double calculate_sigma_k(int k, double sigma_min, double sigma_max) const ;

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

vector<vector<int> > spatialDispersal(int k, vector<int> s, double sigma_min, double sigma_max);
vector<vector<int> > spatialDispersal(vector<int> s, double sigma_min, double sigma_max, ALG2::Sigma &sig); // 使用时间进行sigma的计算，采用重载
#endif //ALG2_H
