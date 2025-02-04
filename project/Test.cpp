//
// Created by 周煜杰 on 25-2-2.
//

#include "Test.h"
#include <unistd.h>
#include <algorithm>
#include <fstream>
#include <sstream>

static mt19937 Rand(getpid());

namespace Test {
    static std::string makespanTestOutput = "";
    namespace Original {
        // 原始算法
        namespace ALG1 {
            vector<int> NEH_PI(const int lambda, const int n, vector<int> &Best_Sequence, const vector<vector<int> > &processing_time) {
                vector<int> order(n - lambda + 1); // 当前顺序，注意范围

                copy(Best_Sequence.begin() + 1, Best_Sequence.begin() + n - lambda + 1, order.begin() + 1);

                for (int i = n - lambda + 1; i <= n; ++i) // 处理后 lambda 个组件
                {
                    int indice = Best_Sequence[i]; // 取出当前组件
                    int index = 0;
                    int bestmakespan = INT_MAX;
                    for (int j = 1; j <= i; ++j) // 插入位置的尝试
                    {
                        vector<int> temp_order = order;
                        temp_order.insert(temp_order.begin() + j, indice); // 将组件插入到位置 j
                        int makespan = Utils::calculate(temp_order, processing_time); // 计算插入后的处理时间
                        if (makespan < bestmakespan) // 更新最优顺序
                        {
                            index = j;
                            bestmakespan = makespan;
                        }
                    }
                    order.insert(order.begin() + index, indice); // 将组件插入到位置 j
                }
                order[0] = Utils::calculate(order, processing_time);
                return order; // 返回最优顺序
            }

            vector<int> Population_Initialization(const int lambda, const int k, const vector<vector<int> > &processing_time, vector<int> indice) {
                int n = indice.size() - 1; // 组件数量
                vector<int> order(2, 0); // 初始化顺序
                int m = processing_time[0].size() - 1; // 机器数量 (machine count)
                vector<vector<int> > d(n + 1, vector<int>(m + 1, 0));

                order[1] = indice[k]; //初始化

                // 初始化
                d[1][0] = 0;

                // 计算d[1][j]
                for (int j = 1; j <= m; ++j) {
                    d[1][j] = d[1][j - 1] + processing_time[order[1]][j];
                }

                indice.erase(indice.begin() + k); // 移除已插入的组件

                for (int i = 2; i <= n; i++) // 直到所有组件都被插入
                {
                    int min_v = INT_MAX;
                    int best_indice = 0; // 最优组件
                    vector<vector<int> > d_best(n + 1, vector<int>(m + 1, 0)); //最佳顺序的d

                    for (int j = 1; j < indice.size(); ++j) // 尝试每个剩余的组件
                    {
                        order.push_back(indice[j]);
                        int v = 0;
                        vector<vector<int> > d_now;
                        d_now = Utils::calculate_v(v, i, order, d, processing_time); // 计算阻塞和空闲时间之和
                        if (min_v > v) // 更新最优组件
                        {
                            best_indice = j;
                            min_v = v;
                            d_best = d_now;
                        }
                        order.pop_back(); // 回退
                    }
                    order.push_back(indice[best_indice]); // 插入最优组件
                    d = d_best;
                    indice.erase(indice.begin() + best_indice); // 移除已插入的组件
                }
                return Test::Original::ALG1::NEH_PI(lambda, n, order, processing_time); // 使用 NEH_PI 算法优化顺序
            }


            vector<vector<int> > ALG1(int N0, int lambda, int x) {
                // 初始化最佳顺序
                vector<int> Best_sequence = Test::Original::ALG1::Population_Initialization(lambda, 1, globalData.processing_time, globalData.indice);

                for (int i = 2; i <= x; ++i) // 生成 x 种顺序并选择最佳
                {
                    vector<int> sequence = Test::Original::ALG1::Population_Initialization(lambda, i, globalData.processing_time, globalData.indice);
                    if (sequence[0] < Best_sequence[0])
                        Best_sequence = sequence; //选取最优
                }

                // makespan输出
                makespanTestOutput += "ALG1_Pop_initialization: ";
                for_each(globalData.POP.begin(), globalData.POP.end(), [&](const auto &i) -> void {
                    makespanTestOutput += to_string(i[0]) + " ";
                });

                globalData.POP.push_back(Best_sequence); //加入种群

                // 输出最佳顺序及其最优时间跨度
                cout << "sequence:";
                for (int i = 1; i <= globalData.n; ++i)
                    cout << " " << Best_sequence[i];
                cout << endl << "minmakespan:" << Best_sequence[0] << endl;

                //完成种群初始化
                for (int i = 2; i <= N0; i++) {
                    vector<int> seq;
                    bool unique = true;
                    do {
                        seq = Utils::random_init(globalData.n); // 生成随机排列
                        seq[0] = Utils::calculate(seq, globalData.processing_time); //计算时间
                        unique = Utils::is_unique(seq, globalData.POP); // 检查是否唯一
                    } while (!unique);
                    globalData.POP.push_back(seq); // 加入POP
                }

                //排序
                Utils::sortAllpi(globalData.POP);

                //设置当前最优解
                globalData.best_seq = globalData.POP[0];
                globalData.bestmakespan = globalData.best_seq[0];

            #ifdef IO_SHOW_PROCESSING_DATA
                //测试POP
                cout<<"=================POP AFTER ALG1:===================="<<endl;
                for (const auto &ele: globalData.POP) {
                    for (auto e: ele) {
                        cout << e << " ";
                    }
                    cout << endl;
                }
                cout<<"====================================================="<<endl;
            #endif


                // makespan输出
                makespanTestOutput += "ALG1_end: ";
                for_each(globalData.POP.begin(), globalData.POP.end(), [&](const auto &i) -> void {
                    makespanTestOutput += to_string(i[0]) + " ";
                });
                makespanTestOutput += "Best_seq_makespan: ";
                makespanTestOutput += to_string(globalData.best_seq[0]) + " ";


                return globalData.POP;
            }
        }

        namespace ALG2{
            vector<vector<int> > spatialDispersal(int k, vector<int> s, const double sigma_min, const double sigma_max) {
                vector<vector<int> > &pop = globalData.POP;
                const vector<vector<int> > processing_time = globalData.processing_time;

                ::ALG2::Sigma sig(pop); //构造sig实例对象

                sig.sigma_k = sig.calculate_sigma_k(k, sigma_min, sigma_max); //TODO:substitute k

                sig.calculate_sigma_i_k();

                for (int i = 0; i < pop.size(); ++i) {
                    for (int j = 0; j < s[i]; j++) {
                        int d = ::ALG2::generate_d(sig, i, sigma_min, sigma_max);
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

                        for (int l = 0; l < d; l++) {//TODO:optimize here
                            int best_time = INT_MAX;
                            vector<int> best_pi;
                            //对不同位置插入pi_R中的元素
                            for (int pos = 1; pos <= pi_new.size(); pos++) {
                                if (pos == pi_new.size()) {
                                    pi_new.push_back(pi_R[l]);
                                    int t = Utils::calculate(pi_new, processing_time);
                                    if (t < best_time) {
                                        best_time = t;
                                        best_pi = pi_new;
                                    }
                                    pi_new.pop_back();
                                } else {
                                    pi_new.insert(pi_new.begin() + pos, pi_R[l]);
                                    int t = Utils::calculate(pi_new, processing_time);
                                    if (t < best_time) {
                                        best_time = t;
                                        best_pi = pi_new;
                                    }
                                    pi_new.erase(pi_new.begin() + pos);
                                }
                            }
                            pi_new = best_pi;
                            pi_new[0] = Utils::calculate(pi_new, processing_time);
                        }
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

            vector<vector<int> > spatialDispersal(vector<int> s, const double sigma_min, const double sigma_max, ::ALG2::Sigma &sig) {
                vector<vector<int> > &pop = globalData.POP;
                const vector<vector<int> > processing_time = globalData.processing_time;

                sig.resetPOP(pop);

                sig.sigma_k = sig.calculate_sigma_k(sigma_min, sigma_max);

                sig.calculate_sigma_i_k();

                for (int i = 0; i < pop.size(); ++i) {
                    for (int j = 0; j < s[i]; j++) {
                        int d = ::ALG2::generate_d(sig, i, sigma_min, sigma_max);
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

                        for (int l = 0; l < d; l++) {//TODO:optimize here
                            int best_time = INT_MAX;
                            vector<int> best_pi;
                            //对不同位置插入pi_R中的元素
                            for (int pos = 1; pos <= pi_new.size(); pos++) {
                                if (pos == pi_new.size()) {
                                    pi_new.push_back(pi_R[l]);
                                    int t = Utils::calculate(pi_new, processing_time);
                                    if (t < best_time) {
                                        best_time = t;
                                        best_pi = pi_new;
                                    }
                                    pi_new.pop_back();
                                } else {
                                    pi_new.insert(pi_new.begin() + pos, pi_R[l]);
                                    int t = Utils::calculate(pi_new, processing_time);
                                    if (t < best_time) {
                                        best_time = t;
                                        best_pi = pi_new;
                                    }
                                    pi_new.erase(pi_new.begin() + pos);
                                }
                            }
                            pi_new = best_pi;
                            pi_new[0] = Utils::calculate(pi_new, processing_time);
                        }
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
        }
    }

    namespace Optimize {
        namespace ALG1 {
           vector<int> NEH_PI(const int lambda, const int n, vector<int> &Best_Sequence, const vector<vector<int>> &processing_time) // 改完后，每次进行寻找最佳邻域直至无法优化，lambda也没用，原来的是在插入过程中逐步更新，每步不一定是最优
            {

                // 初始化
                int m = processing_time[0].size() - 1;
                vector<int> order = Best_Sequence;
                int bestmakespan = INT_MAX;

                vector<vector<int>> e(n + 1, vector<int>(m + 1, 0)), e_2(n + 1, vector<int>(m + 1, 0));
                vector<vector<int>> f(n + 1, vector<int>(m + 1, 0)), f_2(n + 1, vector<int>(m + 1, 0));

                bool improvement = true;

                while (improvement)
                {
                    vector<vector<int>> V(n + 1, vector<int>(n + 1, 1));
                    improvement = false;

                    Utils::calculate_depature_time(e, 1, n, order, processing_time); // 根据当前序列计算e
                    Utils::caculate_tail_time(f, 1, order, processing_time);         // 根据当前序列计算f
                    bestmakespan = f[1][1];                                   // 当前序列的makespan

                    Utils::remove_non_improving_moves(e, f, bestmakespan, V, order, processing_time);

                    int indice = 1;
                    int index = 1;

                    for (int i = 1; i <= n; ++i)
                    {
                        vector<int> temp_order = order;
                        temp_order.erase(temp_order.begin() + i); // 形成pi''序列

                        // 获取e''和f''
                        e_2 = e;
                        f_2 = f;

                        Utils::calculate_depature_time(e_2, i, n - 1, temp_order, processing_time);
                        // e_2.erase(e_2.begin() + n); // 去掉多余数据，不去也没关系
                        Utils::caculate_tail_time(f_2, i, temp_order, processing_time);
                        // f_2.erase(f_2.begin() + n); // 去掉多余数据，不去也没关系

                        // 选择插入位置
                        for (int q = 1; q <= n; ++q)
                        {
                            if (V[i][q] && q != i - 1 && q != i)
                            {
                                vector<vector<int>> e_1 = e_2;
                                temp_order.insert(temp_order.begin() + q, order[i]);             // 插入位置
                                Utils::calculate_depature_time(e_1, q, q, temp_order, processing_time); // 计算e'(q,k)
                                temp_order.erase(temp_order.begin() + q);                        // 还原为pi''序列
                                int makespan = Utils::calculate_makespan(q, e_1, f_2);
                                if (makespan < bestmakespan)
                                {
                                    improvement = true;
                                    bestmakespan = makespan;
                                    indice = i;
                                    index = q;
                                }
                            }
                        }
                    }

                    if (improvement)
                    {
                        int k = order[indice];
                        order.erase(order.begin() + indice);
                        order.insert(order.begin() + index, k);
                    }
                }

                order[0] = bestmakespan;

                return order;
            }

            // 初始化种群，生成一个初始的优良序列，需修改
            vector<int> Population_Initialization(const int lambda, const int k, const vector<vector<int> > &processing_time, vector<int> indice) {
                int n = indice.size() - 1; // 组件数量
                vector<int> order(2, 0); // 初始化顺序
                int m = processing_time[0].size() - 1; // 机器数量 (machine count)
                vector<vector<int> > d(n + 1, vector<int>(m + 1, 0));

                order[1] = indice[k]; //初始化

                // 初始化
                d[1][0] = 0;

                // 计算d[1][j]
                for (int j = 1; j <= m; ++j) {
                    d[1][j] = d[1][j - 1] + processing_time[order[1]][j];
                }

                indice.erase(indice.begin() + k); // 移除已插入的组件

                for (int i = 2; i <= n; i++) // 直到所有组件都被插入
                {
                    int min_v = INT_MAX;
                    int best_indice = 0; // 最优组件
                    vector<vector<int> > d_best(n + 1, vector<int>(m + 1, 0)); //最佳顺序的d

                    for (int j = 1; j < indice.size(); ++j) // 尝试每个剩余的组件
                    {
                        order.push_back(indice[j]);
                        int v = 0;
                        vector<vector<int> > d_now;
                        d_now = Utils::calculate_v(v, i, order, d, processing_time); // 计算阻塞和空闲时间之和
                        if (min_v > v) // 更新最优组件
                        {
                            best_indice = j;
                            min_v = v;
                            d_best = d_now;
                        }
                        order.pop_back(); // 回退
                    }
                    order.push_back(indice[best_indice]); // 插入最优组件
                    d = d_best;
                    indice.erase(indice.begin() + best_indice); // 移除已插入的组件
                }
                return Test::Optimize::ALG1::NEH_PI(lambda, n, order, processing_time); // 使用 NEH_PI 算法优化顺序
            }


            vector<vector<int> > ALG1(int N0, int lambda, int x) {
                // 初始化最佳顺序
                vector<int> Best_sequence = Test::Optimize::ALG1::Population_Initialization(lambda, 1, globalData.processing_time, globalData.indice);

                for (int i = 2; i <= x; ++i) // 生成 x 种顺序并选择最佳
                {
                    vector<int> sequence = Test::Optimize::ALG1::Population_Initialization(lambda, i, globalData.processing_time, globalData.indice);
                    if (sequence[0] < Best_sequence[0])
                        Best_sequence = sequence; //选取最优
                }

                // makespan输出
                makespanTestOutput += "ALG1_Pop_initialization: ";
                for_each(globalData.POP.begin(), globalData.POP.end(), [&](const auto &i) -> void {
                    makespanTestOutput += to_string(i[0]) + " ";
                });

                globalData.POP.push_back(Best_sequence); //加入种群

                // 输出最佳顺序及其最优时间跨度
                cout << "sequence:";
                for (int i = 1; i <= globalData.n; ++i)
                    cout << " " << Best_sequence[i];
                cout << endl << "minmakespan:" << Best_sequence[0] << endl;

                //完成种群初始化
                for (int i = 2; i <= N0; i++) {
                    vector<int> seq;
                    bool unique = true;
                    do {
                        seq = Utils::random_init(globalData.n); // 生成随机排列
                        seq[0] = Utils::calculate(seq, globalData.processing_time); //计算时间
                        unique = Utils::is_unique(seq, globalData.POP); // 检查是否唯一
                    } while (!unique);
                    globalData.POP.push_back(seq); // 加入POP
                }

                //排序
                Utils::sortAllpi(globalData.POP);

                //设置当前最优解
                globalData.best_seq = globalData.POP[0];
                globalData.bestmakespan = globalData.best_seq[0];

            #ifdef IO_SHOW_PROCESSING_DATA
                //测试POP
                cout<<"=================POP AFTER ALG1:===================="<<endl;
                for (const auto &ele: globalData.POP) {
                    for (auto e: ele) {
                        cout << e << " ";
                    }
                    cout << endl;
                }
                cout<<"====================================================="<<endl;
            #endif

                // makespan输出
                makespanTestOutput += "ALG1_end: ";
                for_each(globalData.POP.begin(), globalData.POP.end(), [&](const auto &i) -> void {
                    makespanTestOutput += to_string(i[0]) + " ";
                });
               makespanTestOutput += "Best_seq_makespan: ";
               makespanTestOutput += to_string(globalData.best_seq[0]) + " ";


                return globalData.POP;
            }
        }
    }
    /*
     * 比较两个字符串是否相等，不相等则返回第一个不相等的位置
     */
    size_t stringCompare(const std::string& s1, const std::string& s2) {
        auto mismatch_pair = std::mismatch(s1.begin(), s1.end(), s2.begin());
        if (mismatch_pair.first == s1.end() && mismatch_pair.second == s2.end())
            return std::string::npos;
        return std::distance(s1.begin(), mismatch_pair.first);
    }

    /*
     * 比较获取随机用例
     */
    std::vector<std::vector<int>> generateExample() {
        // 初始化随机数
        std::default_random_engine e(time(0));
        std::uniform_int_distribution<int> n_rand(1, 100);
        std::uniform_int_distribution<int> m_rand(1, 20);

        // 随机产生机器数和工件数
        int n = n_rand(e);
        int m = m_rand(e);
        // int n = 50;
        // int m = 100;

        // 产生工件加工时间的随机序列
        std::vector<std::vector<int>> example;
        for (int i = 0; i < m; i ++) {
            std::vector<int> tmp(n, 0);
            for (auto &i : tmp)
                i = Rand()%100 + 1;
            example.push_back(tmp);
        }
        return example;
    }

    /*
     * 设置globalData的数据
     */
    pair<int, int> setData(const std::vector<std::vector<int>> &example) {
        const int n = example[0].size();
        const int m = example.size();
        globalData.n = n;
        globalData.m = m;

#ifdef IO_SHOW_INPUT_DATA
        std::cout << "The number of jobs: " << n << std::endl;
        std::cout << "The number of machines: " << m << std::endl;
#endif
        globalData.processing_time.assign(n+1, std::vector<int>(m+1, 0));
        globalData.total_processing_time.assign(n+1, 0);
        globalData.indice.assign(n+1, 0);
        std::iota(globalData.indice.begin(), globalData.indice.end(), 0);

#ifdef  IO_SHOW_INPUT_DATA
        std::cout << "\nThe processing time of each component on different machines: \n";
        for_each(example.begin(), example.end(), [](const vector<int> &i) -> void {
            for_each(i.begin(), i.end(), [](const int &i) -> void {
                std::cout << i << "\t";
            });
            std::cout << std::endl;
        });
#endif
        for (int i = 1; i <= m; i ++) {
            for (int j = 1; j <= n; j ++) {
                globalData.processing_time[j][i] = example[i-1][j-1];
                globalData.total_processing_time[j] += globalData.processing_time[j][i];
            }
        }
        Utils::sort_by_tot_processing_time(globalData.indice, globalData.total_processing_time);
        return {n, m};
    }


    const std::string oriAlgorithm(const std::vector<std::vector<int>> &example) {
        makespanTestOutput = "";
        globalData.resetData();
        auto[jobNum, machineNum] = setData(example);

        ::ALG2::Sigma sig;
        Test::Original::ALG1::ALG1(10, 10, 5);

        while (sig.getRunTime() <= ::ALG2::t_max) {
            vector<int> s = reproduction(0, 7);
            globalData.POP2.clear();
            for (int i = 1; i <= globalData.P_max; i ++) {
                Test::Original::ALG2::spatialDispersal(s, 0, 5, sig);
            }

            for (int i = 0; i < globalData.POP2.size(); i ++) {
                std::default_random_engine e(time(0));
                std::uniform_real_distribution<double> _rand(0, 1);
                double rand = _rand(e);

                if (rand < globalData.pls) {
                    globalData.POP2[i] = SRLS(globalData.POP2[i], globalData.best_seq);
                }
            }


            // makespan输出
            makespanTestOutput += "ALG2-3: ";
            for_each(globalData.POP2.begin(), globalData.POP2.end(), [&](const auto &i) -> void {
                makespanTestOutput += to_string(i[0]) + " ";
            });

            competitiveExclusion(globalData.POP, globalData.POP2, globalData.P_max);

            // makespan输出
            makespanTestOutput += "ALG4: ";
            for_each(globalData.POP.begin(), globalData.POP.end(), [&](const auto &i) -> void {
                makespanTestOutput += to_string(i[0]) + " ";
            });
        }


        makespanTestOutput += "Result: ";
        makespanTestOutput += to_string(globalData.best_seq[0]);

        return makespanTestOutput;
    }

    /*
     * 返回新的算法的makespan组成的字符串
     */
    const std::string newAlgorithm(const std::vector<std::vector<int>> &example) {
        makespanTestOutput = "";
        globalData.resetData();
        auto[jobNum, machineNum] = setData(example);

        ::ALG2::Sigma sig;
        Test::Optimize::ALG1::ALG1(10, 10, 5);

        while (sig.getRunTime() <= ::ALG2::t_max) {

            vector<int> s = reproduction(0, 7);
            globalData.POP2.clear();
            for (int i = 1; i <= globalData.P_max; i ++) {
                spatialDispersal(s, 0, 5, sig);
            }

            for (int i = 0; i < globalData.POP2.size(); i ++) {
                std::default_random_engine e(time(0));
                std::uniform_real_distribution<double> _rand(0, 1);
                double rand = _rand(e);

                if (rand < globalData.pls) {
                    globalData.POP2[i] = SRLS(globalData.POP2[i], globalData.best_seq);
                }
            }


            // makespan输出
            makespanTestOutput += "ALG2-3: ";
            for_each(globalData.POP2.begin(), globalData.POP2.end(), [&](const auto &i) -> void {
                makespanTestOutput += to_string(i[0]) + " ";
            });


            competitiveExclusion(globalData.POP, globalData.POP2, globalData.P_max);

            // makespan输出
            makespanTestOutput += "ALG4: ";
            for_each(globalData.POP.begin(), globalData.POP.end(), [&](const auto &i) -> void {
                makespanTestOutput += to_string(i[0]) + " ";
            });
        }

        makespanTestOutput += "Result: ";
        makespanTestOutput += to_string(globalData.best_seq[0]);
        return makespanTestOutput;
    }

    bool loadFailedTestsAndTest() {
        ifstream fIn("failed_tests.txt");
        if (!fIn.is_open())
            throw domain_error("makespanTest-loadFailedTestsAndTest: Failed to open failed_tests.txt!\n");
        std::string line;
        while (getline(fIn, line)) {

            // 读取原来的用例
            std::vector<std::vector<int>> example;
            getline(fIn, line);
            istringstream iss(line);
            int n, m;
            iss >> n >> m;
            for (int i = 0; i < m; i ++) {
                getline(fIn, line);
                istringstream iss1(line);
                std::vector<int> tmp;
                for (int j = 0; j < n; j ++) {
                    int num;
                    iss1 >> num;
                    tmp.push_back(num);
                }
                example.push_back(tmp);
            }

            getline(fIn, line);
            getline(fIn, line);
            getline(fIn, line);

            std::string exampleString = "";

            for_each(example.begin(), example.end(), [&](const auto &i) -> void {
                            for_each(i.begin(), i.end(), [&](const auto &i) -> void {
                                exampleString  = exampleString + to_string(i) + "\t";
                            });
                            exampleString  = exampleString + "\n";
                        });
            std::string newStr = newAlgorithm(example);
            std::string oriStr = oriAlgorithm(example);

            size_t pos = stringCompare(newStr, oriStr);
            if (pos == std::string::npos)
                continue;
            else {
                fIn.close();
                ofstream fOut("failed_tests.txt", std::ios::app);   // 用于测试未通过的案例
                if (!fOut.is_open())
                    throw std::domain_error("makespanTest: Failed to open failed_tests.txt!\n");

                fOut << "n, m, Test case: " << "\n"
                     << example[0].size() << " " << example.size() << "\n"
                     << exampleString;
                newStr.insert(pos, "][");
                oriStr.insert(pos, "][");
                fOut << "Output:\n" << "New:\t" << newStr << "\n" << "Ori:\t" << oriStr << std::endl;
                fOut.close();
                return false;
            }
        }
        return true;
    }

    bool makespanTest(int testCounts) {
        if (testCounts < 0)
            throw std::domain_error("makespanTest: testCounts < 0!\n");

        ofstream fOut("failed_tests.txt", std::ios::app);   // 用于测试未通过的案例
        if (!fOut.is_open())
            throw std::domain_error("makespanTest: Failed to open failed_tests.txt!\n");

        bool passTest = false;

        // 测试之前未通过的案例是否通过
        passTest = loadFailedTestsAndTest();
        if (!passTest) {
            fOut.close();
            return passTest;
        }

        std::string exampleString = ""; // 用于存储用例的字符串，用于后期打印输出

        for (int i = 0; i < testCounts; i ++) {
            const std::vector<std::vector<int>> example = generateExample();    // 产生随机用例
            // 用于存储用例字符串
            for_each(example.begin(), example.end(), [&](const auto &i) -> void {
                for_each(i.begin(), i.end(), [&](const auto &i) -> void {
                    exampleString  = exampleString + to_string(i) + "\t";
                });
                exampleString  = exampleString + "\n";
            });
            std::string newStr = newAlgorithm(example);
            std::string oriStr = oriAlgorithm(example);

            size_t pos = stringCompare(newStr, oriStr);
            if (pos == std::string::npos)
                continue;
            else {
                fOut << "n, m, Test case: " << "\n"
                     << example[0].size() << " " << example.size() << "\n"
                     << exampleString;
                newStr.insert(pos, "][");
                oriStr.insert(pos, "][");
                fOut << "Output:\n" << "New:\t" << newStr << "\n" << "Ori:\t" << oriStr << std::endl;
                fOut.close();
                passTest = false;
                return passTest;
            }

        }
        fOut.close();
        passTest = true;
        return passTest;
    }
}

#ifdef IO_SHOW_INPUT_DATA
#undef IO_SHOW_INPUT_DATA
#endif

#ifdef IO_SHOW_PROCESSING_DATA
#undef IO_SHOW_PROCESSING_DATA
#endif

#ifdef IO_SHOW_PROCESSING_FILE
#undef IO_SHOW_PROCESSING_FILE
#endif

#ifdef IO_SHOW_FINAL_RESULT
#undef IO_SHOW_FINAL_RESULT
#endif
