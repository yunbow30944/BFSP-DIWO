#include <random>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <vector>


using std::cin;
using std::cout;
using std::vector;

namespace ALG2 {
    const double epsilon = 1e-7;
    const vector<int> S_MIN{ 0, 1, 2 };
    const vector<int> S_MAX{ 5, 7, 9 };
    const vector<int> SIGMA_MIN{ 3, 4, 5 };
    const vector<int> SIGMA_MAX{ 8, 9, 10 };

    double calculate_C_max(const vector<int>& pi, const vector<vector<int>>& processing_time){
        int numJobs = pi.size();
        int numMachines = processing_time.size();
        std::vector<int> completionTimes(numMachines, 0);

        // 初始化第一台机器的完成时间
        for (int job = 0; job < numJobs; ++job) {
            completionTimes[0] += processing_time[pi[job]][0];
        }

        // 计算每台机器上每个作业的完成时间
        for (int machine = 1; machine < numMachines; ++machine) {
            for (int job = 0; job < numJobs; ++job) {
                int jobIndex = pi[job];
                // 作业必须等待前一台机器上的所有作业完成
                int startTime = std::max(completionTimes[machine - 1], completionTimes[machine]);
                completionTimes[machine] = startTime + processing_time[jobIndex][machine];
            }
        }

        // Cmax 是最后一台机器上所有作业的最大完成时间
        return *std::max_element(completionTimes.begin(), completionTimes.end());

    }

    double calculate_S(const vector<int>& pi_index, const vector<vector<int>> &processing_time, const vector<int>& pi_worst, const vector<int>& pi_best, double S_max, double S_min) {
        return ((calculate_C_max(pi_worst, processing_time) - calculate_C_max(pi_index, processing_time) + epsilon) / (calculate_C_max(pi_worst, processing_time) - calculate_C_max(pi_best, processing_time) + epsilon)) * (S_max - S_min) + S_min;
    }

    int calculate(const vector<int>& order_original, const vector<vector<int>>& processing_time) {
        vector<int> order(order_original.size() + 1, 0); // 为了方便计算，将order的下标从1开始
        for (int i = 1; i < order.size(); ++i)
            order[i] = order_original[i - 1];
		//？？？
        int n = order.size() - 1; // 组件数量
        int m = processing_time[0].size() - 1; // 机器数量
        vector<int> finish_time(m + 1, 0); // 每台机器的完成时间
        for (int i = 1; i <= n; ++i) // 遍历每个组件
            for (int j = 1; j <= m; ++j) // 遍历每台机器
            {
                if (i == 1)
                    finish_time[j] = finish_time[j - 1] + processing_time[order[i]][j]; // 第一个组件
                else if (j != m)
                    finish_time[j] = std::max(finish_time[j - 1], finish_time[j + 1] - processing_time[order[i - 1]][j + 1]) + processing_time[order[i]][j]; // 中间机器
                else
                    finish_time[j] = std::max(finish_time[j - 1], finish_time[j]) + processing_time[order[i]][j]; // 最后一台机器
            }
        return finish_time[m]; // 返回最后一台机器的完成时间
    }

    class Sigma {
    private:
        vector<vector<int>> pop;
        vector<vector<int>> sigma_i_k;
        clock_t start;//此处CPU最大执行时间与算法开始时间均用start表示
        double calculate_sigma_i_k(double sigma_min, double sigma_max) {
            return (1-(static_cast<double>(clock()-start))/start)*(sigma_max-sigma_min)+sigma_min;
        }
    public:
        Sigma() {
            start = clock();
        }
        Sigma(const vector<vector<int>>& pop) : pop(pop) {
            start = clock();
        }
        ~Sigma() = default;
        vector<vector<int>>& get_sigma() {
            return this->sigma_i_k;
        }
        void calculate_sigma_i_k(vector<vector<int>> pop_now, const vector<vector<int>> &porcessing_time);
    };

    int justify_d(int d, int n, const double sigma_min, const double sigma_max) {
        if (d <= n / 2 && d >= sigma_min) {
            return d;
        }
        else {
            while (!(d > static_cast<double>(n / 2) || d < static_cast<double>(sigma_min))) {
                std::default_random_engine e(time(0));
                std::uniform_real_distribution<double> _rand(0, 1);
                d = static_cast<int>((sigma_min + _rand(e) * (sigma_max - sigma_min)));
            }
            return d;
        }
    }

    int generate_d(Sigma& sigma, int i, int k, const double sigma_min, const double sigma_max) {
        int d = 0;
        std::default_random_engine e(time(0));
        std::normal_distribution<double> _rand(0, ((sigma.get_sigma())[k][i]) * ((sigma.get_sigma())[k][i]));
        d = static_cast<int>(fabs(_rand(e)));
        return justify_d(d, (sigma.get_sigma())[0].size(), sigma_min, sigma_max);
    }



    void Sigma::calculate_sigma_i_k(vector<vector<int>> pop_now, const vector<vector<int>> &processing_time) {
        vector<std::pair<int, double>> sigma_i;
        pop = pop_now;
        vector<vector<int>> tmp(pop.size(), vector<int>(pop[0].size(), 0));
        sigma_i_k = tmp;

        for(int i = 0; i < pop.size(); ++i) {
            double C_i_j = calculate_C_max(pop[i], processing_time);
            sigma_i.push_back(std::make_pair(i, C_i_j));
        }
        std::sort(sigma_i.begin(), sigma_i.end(), [](std::pair<int, double> a, std::pair<int, double> b){
            return a.second < b.second;
        });
        double median = 0;
        if(sigma_i.size()%2){
            median = sigma_i[sigma_i.size()/2].second;
        }else{
            median = (sigma_i[sigma_i.size()/2].second + sigma_i[sigma_i.size()/2-1].second)/2;
        }
        for(const auto &i : sigma_i){
            if(i.second < median){
                for(int h = 0; h < sigma_i_k[0].size(); h ++){
                    sigma_i_k[i.first][h] = i.second;
                }
            }else{
                for(int h = 0; h < sigma_i_k[0].size(); h ++){
                    sigma_i_k[i.first][h] = i.second*((calculate_C_max(pop[i.first], processing_time)-calculate_C_max(pop[sigma_i.size()/2], processing_time))/(calculate_C_max(pop[pop.size()-1], processing_time)-calculate_C_max(pop[sigma_i.size()/2], processing_time))+epsilon*0.5+1);
                }
            }
        }
    }

    vector<int> sort_by_non_decreasing_processing_time(const vector<int>& pi_R, const vector<vector<int>>& processing_time) {
        vector<int> best, tmp(pi_R);
        // vector<vector<int>> all_permutation;
        std::sort(tmp.begin(), tmp.end(), [](int a, int b) -> bool {
            return a < b;
            });
        int best_time = INT_MAX;
        do {
            int t = calculate(tmp, processing_time);
            if (t < best_time) {
                best_time = t;
                best = tmp;
            }
        } while (std::next_permutation(tmp.begin(), tmp.end()));
        // vector<int> total_processsing_time(all_permutation.size(), 0);

        return best;
    }
}

vector<vector<int>> randomInsertation(vector<vector<int>> pop, const vector<vector<int>>& processing_time, const double S_min, const double S_max, const double sigma_min, const double sigma_max) {
    ALG2::Sigma sig(pop);
    vector<double> s(pop.size());
    for (const auto& pi_index : pop) {
        // POP输入时需要根据排列的好坏程度先进行降序排列，默认pop[0]是最好的，pop[pop.size()-1]是最差的
        double S_index = ALG2::calculate_S(pi_index, processing_time, pop[pop.size() - 1], pop[0],  S_max, S_min);
        s.push_back(S_index);
    }
    for (int i = 0; i < pop.size(); ++i) {
        sig.calculate_sigma_i_k(pop, processing_time);
        for (int j = 0; j < s[i]; j++) {
            int d = ALG2::generate_d(sig, i, j, sigma_min, sigma_max);
            vector<int> pi_R;
            vector<int> pi_new(pop[i]);
            for (int m = 0; m < d; m++) {
                std::default_random_engine e(time(0));
                std::uniform_int_distribution<int> _rand(0, pop[i].size() - m);
                int position = _rand(e);
                pi_R.push_back(pop[i][position]);
                pi_new.erase(pop[i].begin() + position);
            }
            pi_R = ALG2::sort_by_non_decreasing_processing_time(pi_R, processing_time);
            for (int k = 0; k < d; k++) {
                int best_time = INT_MAX;
                vector<int> best_pi;
                //对不同位置插入pi_R中的元素
                for (int pos = 0; pos <= pi_new.size(); pos++) {
                    if (pos == pi_new.size()) {
                        pi_new.push_back(pi_R[k]);
                        int t = ALG2::calculate(pi_new, processing_time);
                        if (t < best_time) {
                            best_time = t;
                            best_pi = pi_new;
                            pi_new.pop_back();
                        }
                        else {
                            pi_new.pop_back();
                        }
                    }
                    else {
                        pi_new.insert(pi_new.begin() + pos, pi_R[k]);
                        int t = ALG2::calculate(pi_new, processing_time);
                        if (t < best_time) {
                            best_time = t;
                            best_pi = pi_new;
                            pi_new.erase(pi_new.begin() + pos);
                        }
                        else {
                            pi_new.erase(pi_new.begin() + pos);
                        }
                    }
                }
                pi_new = best_pi;
            }
            pop.push_back(pi_new);
        }
    }
    return pop;
}
