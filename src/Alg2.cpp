#include"Alg1.h"
#include"Alg2.h"
#include"Utils.h"
#include"GlobalData.h"
//#define IO_SHOW_PROCESSING_DATA
using std::cin;
using std::cout;
using std::vector;

namespace ALG2 {
    int calculate_S(int pi_now, int pi_worst, int pi_best, int S_max, int S_min) {
        return (int) (((pi_worst - pi_now + epsilon) / (pi_worst - pi_best + epsilon)) * (S_max - S_min) + S_min);
    }

    int justify_d(int d, const double sigma_min, const double sigma_max) {
        int n=globalData.n;
        if (d <= n / 2 && d >= sigma_min) {
            return d;
        } else {
            while (d > static_cast<double>(n / 2) || d < static_cast<double>(sigma_min)) {
                std::default_random_engine e(time(0));
                std::uniform_real_distribution<double> _rand(0, 1);
                d = static_cast<int>(sigma_min + _rand(e) * (sigma_max - sigma_min));
            }
            return d;
        }
    }

    int generate_d(Sigma &sigma, int i, const double sigma_min, const double sigma_max) {
        int d = 0;
        std::default_random_engine e(time(0));
        std::normal_distribution<double> _rand(0, sigma.sigma_i_k[i]);
        d = static_cast<int>(fabs(_rand(e)));
        return justify_d(d, sigma_min, sigma_max);
    }

    double Sigma::calculate_sigma_k(double sigma_min, double sigma_max) const {
        return (1 - (static_cast<double>(clock() - start)) / t_max) * (sigma_max - sigma_min) + sigma_min;
    }

    void Sigma::calculate_sigma_i_k() {
        vector<vector<int> > &pop = globalData.POP;
        double C_median = 0;
        double C_worst = pop[pop.size() - 1][0];

        if (pop.size() % 2) {
            C_median = pop[pop.size() / 2][0];
        } else {
            C_median = 1.0 * (pop[pop.size() / 2][0] + pop[pop.size() / 2 - 1][0]) / 2;
        }

        for (int i = 0; i < pop.size(); i++) {
            if (i < pop.size() / 2) {
                sigma_i_k.push_back(sigma_k);
            } else {
                sigma_i_k.push_back(sigma_k * (1 + 0.5 * ((pop[i][0] - C_median) / (C_worst - C_median + epsilon))));
            }
        }
    }
}

vector<int> reproduction(const int S_min, const int S_max) {
    vector<vector<int> > &pop = globalData.POP;
    vector<int> s;
    for (const auto &pi_index: pop) {
        int S_index = ALG2::calculate_S(pi_index[0], pop[pop.size() - 1][0], pop[0][0], S_max, S_min);
        s.push_back(S_index);
    }
    return s;
}

vector<vector<int> > spatialDispersal(vector<int> s, const double sigma_min, const double sigma_max, ALG2::Sigma &sig) {
    vector<vector<int> > &pop = globalData.POP;
    const vector<vector<int> > processing_time = globalData.processing_time;
    int m = globalData.m;
    int n = globalData.n;

    //sig.resetPOP(pop);

    sig.sigma_k = sig.calculate_sigma_k(sigma_min, sigma_max);

    sig.calculate_sigma_i_k();

    for (int i = 0; i < pop.size(); ++i) {
        //对pop中每个个体
        for (int j = 0; j < s[i]; j++) {
            //适应度S，种子数
            int d = generate_d(sig, i, sigma_min, sigma_max);
            vector<int> pi_R;
            vector<int> pi_new(pop[i]);
            for (int k = 0; k < d; k++) {
                //随机移走d个
                std::default_random_engine e(time(0));
                std::uniform_int_distribution<int> _rand(1, pi_new.size() - 1);
                int position = _rand(e);
                pi_R.push_back(pi_new[position]);
                pi_new.erase(pi_new.begin() + position);
            }
            Utils::sort_by_tot_processing_time(pi_R, globalData.total_processing_time); //升序排序

            vector<vector<int> > e_2(n + 1, vector<int>(m + 1, 0));
            Utils::calculate_departure_time(e_2, 1, n - d, pi_new, processing_time); //得到初始的e''

            vector<vector<int> > f_2(n + 1, vector(m + 2, 0));
            Utils::calculate_tail_time(f_2, n - d, 1, pi_new, processing_time); //得到初始的f''

            for (int l = 0; l < d; l++) {
                //移走的d个重新放回去
                int job = pi_R[l];
                int best_index;
                int bestmakespan = INT_MAX;
                pair<int, int> ans = Utils::neighbor_insertion(n - d + l, job, e_2, f_2, processing_time);
                best_index = ans.first;
                bestmakespan = ans.second;
#ifdef IO_SHOW_PROCESSING_DATA
                cout << "best index: " << best_index << ", best makespan = " << bestmakespan << endl;
#endif
                pi_new.insert(pi_new.begin() + best_index, job); //更新order
                Utils::calculate_departure_time(e_2, best_index, n - d + l + 1, pi_new, processing_time); //更新e''
                //新f''的[best_index+1,n-d+l+1]用原f''的[best_index,n-d+l]
                copy(f_2.begin() + best_index, f_2.begin() + n - d + l + 1, f_2.begin() + best_index + 1);
                Utils::calculate_tail_time(f_2, best_index, 1, pi_new, processing_time); //更新f''
                if (l == d - 1) pi_new[0] = bestmakespan;
            }
            //int truemakespan = Utils::calculate(pi_new,globalData.processing_time);
            //cout<<"truemakespan = "<<truemakespan<<endl;
            //Utils::print_pi(pi_new);
            globalData.POP2.push_back(pi_new); //加入POP'
        }
    }
    vector<int> bestseq = Utils::findBestpi(globalData.POP2);
    if (bestseq[0] < globalData.bestmakespan) {
        globalData.bestmakespan = bestseq[0];
        globalData.best_seq = bestseq;
    }
    return globalData.POP2;
}
