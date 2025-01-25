#include"Alg1.h"
#include"Alg2.h"
#include"Utils.h"
#include"GlobalData.h"

using std::cin;
using std::cout;
using std::vector;

namespace ALG2 {
    int calculate_S(int pi_now, int pi_worst, int pi_best, int S_max, int S_min) {
        return (int) (((pi_worst - pi_now + epsilon) / (pi_worst - pi_best + epsilon)) * (S_max - S_min) + S_min);
    }

    int justify_d(int d, int n, const double sigma_min, const double sigma_max) {
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
        return justify_d(d, globalData.n, sigma_min, sigma_max);//?
    }


    double Sigma::calculate_sigma_k(int k, double sigma_min, double sigma_max) const {
        return sigma_min + (1 - (k - 1) / (globalData.k_max - 1 + epsilon)) * (sigma_max - sigma_min);
    }

    // 重载calculate_sigma_k
    double Sigma::calculate_sigma_k(double sigma_min, double sigma_max) const {
        return (1-(static_cast<double>(clock()-start))/t_max)*(sigma_max-sigma_min)+sigma_min;
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
            if (i < pop.size() / 2) {//TODO: verify this judgement
                sigma_i_k.push_back(sigma_k);
            } else {
                sigma_i_k.push_back(sigma_k * (1 + 0.5 * ((pop[i][0] - C_median) / (C_worst - C_median + epsilon))));
            }
        }
    }
}

vector<int> reproduction(const int S_min, const int S_max) {
    vector<vector<int> > &pop = globalData.POP;
    const vector<vector<int> > processing_time = globalData.processing_time;
    vector<int> s;
    for (const auto &pi_index: pop) {
        int S_index = ALG2::calculate_S(pi_index[0], pop[pop.size() - 1][0], pop[0][0], S_max, S_min);
        s.push_back(S_index);
    }
    return s;
}

vector<vector<int> > spatialDispersal(int k, vector<int> s, const double sigma_min, const double sigma_max) {
    vector<vector<int> > &pop = globalData.POP;
    const vector<vector<int> > processing_time = globalData.processing_time;

    ALG2::Sigma sig(pop); //构造sig实例对象

    sig.sigma_k = sig.calculate_sigma_k(k, sigma_min, sigma_max); //TODO:substitute k

    sig.calculate_sigma_i_k();

    for (int i = 0; i < pop.size(); ++i) {
        for (int j = 0; j < s[i]; j++) {
            int d = generate_d(sig, i, sigma_min, sigma_max);
            vector<int> pi_R;
            vector<int> pi_new(pop[i]);
            for (int m = 0; m < d; m++) {
                std::default_random_engine e(time(0));
                std::uniform_int_distribution<int> _rand(1, pi_new.size() - 1);
                int position = _rand(e);
                pi_R.push_back(pi_new[position]);
                pi_new.erase(pi_new.begin() + position);
            }
            Utils::sort_by_tot_processing_time(pi_R, globalData.total_processing_time);//TODO: 升序真的好吗？

            for (int l = 0; l < d; l++)
            {
                vector<int> best_pi, order = pi_new;
                order.push_back(pi_R[l]); // order是pi序列，pi_new是pi''序列，插入后变成pi'序列

                int n = order.size() - 1;
                int m = processing_time[0].size() - 1;
                
                vector<vector<int>> e(n + 1, vector<int>(m + 1, 0));
                vector<vector<int>> f(n + 1, vector<int>(m + 2, 0));
                vector<vector<int>> V(n + 1, vector<int>(n + 1, 1));

                Utils::calculate_depature_time(e, 1, n, order, processing_time);
                Utils::caculate_tail_time(f, 1, order, processing_time);
                int best_time = f[1][1];
                Utils::remove_non_improving_moves(e, f, best_time, V, order, processing_time);

                vector<vector<int>> e_2 = e;
                vector<vector<int>> f_2 = f;

                Utils::calculate_depature_time(e_2, n, n - 1, pi_new, processing_time);
                Utils::caculate_tail_time(f_2, n, pi_new, processing_time);

                for (int pos = 1; pos < pi_new.size(); pos++)
                {
                    pi_new.insert(pi_new.begin() + pos, pi_R[l]);
                    if (V[n][pos])
                    {
                        vector<vector<int>> e_1 = e_2;
                        pi_new.insert(pi_new.begin() + pos, pi_R[l]);
                        Utils::calculate_depature_time(e_1, pos, pos, pi_new, processing_time);
                        int t = Utils::calculate_makespan(pos, e_1, f_2);
                        if (t < best_time)
                        {
                            best_time = t;
                            best_pi = pi_new;
                        }
                    }
                    pi_new.erase(pi_new.begin() + pos);
                }

                pi_new = best_pi;
                pi_new[0] = best_time;
            }

            // for (int l = 0; l < d; l++) {//TODO:optimize here
            //     int best_time = INT_MAX;
            //     vector<int> best_pi;
            //     //对不同位置插入pi_R中的元素
            //     for (int pos = 1; pos <= pi_new.size(); pos++) {
            //         if (pos == pi_new.size()) {
            //             pi_new.push_back(pi_R[l]);
            //             int t = Utils::calculate(pi_new, processing_time);
            //             if (t < best_time) {
            //                 best_time = t;
            //                 best_pi = pi_new;
            //             }
            //             pi_new.pop_back();
            //         } else {
            //             pi_new.insert(pi_new.begin() + pos, pi_R[l]);
            //             int t = Utils::calculate(pi_new, processing_time);
            //             if (t < best_time) {
            //                 best_time = t;
            //                 best_pi = pi_new;
            //             }
            //             pi_new.erase(pi_new.begin() + pos);
            //         }
            //     }
            //     pi_new = best_pi;
            //     pi_new[0] = Utils::calculate(pi_new, processing_time);
            // }
            globalData.POP2.push_back(pi_new);
        }
    }
    vector<int> bestseq = Utils::findBestpi(globalData.POP2);
    if (bestseq[0] < globalData.bestmakespan) {
        globalData.bestmakespan = bestseq[0];
        globalData.best_seq = bestseq;
    }
    return globalData.POP2;
}

vector<vector<int> > spatialDispersal(vector<int> s, const double sigma_min, const double sigma_max, ALG2::Sigma &sig) {
    vector<vector<int> > &pop = globalData.POP;
    const vector<vector<int> > processing_time = globalData.processing_time;

    sig.resetPOP(pop);

    sig.sigma_k = sig.calculate_sigma_k(sigma_min, sigma_max);

    sig.calculate_sigma_i_k();

    for (int i = 0; i < pop.size(); ++i) {
        for (int j = 0; j < s[i]; j++) {
            int d = generate_d(sig, i, sigma_min, sigma_max);
            vector<int> pi_R;
            vector<int> pi_new(pop[i]);
            for (int m = 0; m < d; m++) {
                std::default_random_engine e(time(0));
                std::uniform_int_distribution<int> _rand(1, pi_new.size() - 1);
                int position = _rand(e);
                pi_R.push_back(pi_new[position]);
                pi_new.erase(pi_new.begin() + position);
            }
            Utils::sort_by_tot_processing_time(pi_R, globalData.total_processing_time);//TODO: 升序真的好吗？

            for (int l = 0; l < d; l++)
            {
                vector<int> best_pi, order = pi_new;
                order.push_back(pi_R[l]);//order是pi序列，pi_new是pi''序列，插入后变成pi'序列

                int n = order.size() - 1;
                int m = processing_time[0].size() - 1;
                
                vector<vector<int>> e(n + 1, vector<int>(m + 1, 0));
                vector<vector<int>> f(n + 1, vector<int>(m + 2, 0));
                vector<vector<int>> V(n + 1, vector<int>(n + 1, 1));

                Utils::calculate_depature_time(e, 1, n, order, processing_time);
                Utils::caculate_tail_time(f, 1, order, processing_time);
                int best_time = f[1][1];
                Utils::remove_non_improving_moves(e, f, best_time, V, order, processing_time);

                vector<vector<int>> e_2 = e;
                vector<vector<int>> f_2 = f;

                Utils::calculate_depature_time(e_2, n,n - 1, pi_new, processing_time);
                Utils::caculate_tail_time(f_2, n, pi_new, processing_time);

                for (int pos = 1; pos < pi_new.size(); pos++)
                {
                    pi_new.insert(pi_new.begin() + pos, pi_R[l]);
                    if(V[n][pos])
                    {
                        vector<vector<int>> e_1 = e_2;
                        pi_new.insert(pi_new.begin() + pos, pi_R[l]);
                        Utils::calculate_depature_time(e_1, pos, pos, pi_new, processing_time);
                        int t = Utils::calculate_makespan(pos, e_1, f_2);
                        if (t<best_time)
                        {
                            best_time = t;
                            best_pi = pi_new;
                        }
                    }
                    pi_new.erase(pi_new.begin() + pos);
                }

                pi_new = best_pi;
                pi_new[0] = best_time;
            }

            // for (int l = 0; l < d; l++) {//TODO:optimize here
            //     int best_time = INT_MAX;
            //     vector<int> best_pi;
            //     //对不同位置插入pi_R中的元素
            //     for (int pos = 1; pos <= pi_new.size(); pos++) {
            //         if (pos == pi_new.size()) {
            //             pi_new.push_back(pi_R[l]);
            //             int t = Utils::calculate(pi_new, processing_time);
            //             if (t < best_time) {
            //                 best_time = t;
            //                 best_pi = pi_new;
            //             }
            //             pi_new.pop_back();
            //         } else {
            //             pi_new.insert(pi_new.begin() + pos, pi_R[l]);
            //             int t = Utils::calculate(pi_new, processing_time);
            //             if (t < best_time) {
            //                 best_time = t;
            //                 best_pi = pi_new;
            //             }
            //             pi_new.erase(pi_new.begin() + pos);
            //         }
            //     }
            //     pi_new = best_pi;
            //     pi_new[0] = Utils::calculate(pi_new, processing_time);
            // }
            globalData.POP2.push_back(pi_new);
        }
    }
    vector<int> bestseq = Utils::findBestpi(globalData.POP2);
    if (bestseq[0] < globalData.bestmakespan) {
        globalData.bestmakespan = bestseq[0];
        globalData.best_seq = bestseq;
    }
    return globalData.POP2;
}
