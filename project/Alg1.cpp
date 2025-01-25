//
// Created by DELL on 24-9-9.
//

#include "Alg1.h"
#include "Utils.h"
#include "GlobalData.h"
#include "IO.h"

using namespace std;

vector<int> NEH_PI(const int lambda, const int n, vector<int> &Best_Sequence,
                   const vector<vector<int>> &processing_time) // 改完后，每次进行寻找最佳邻域直至无法优化，lambda也没用，原来的是在插入过程中逐步更新，每步不一定是最优
{

    // 初始化
    int m = processing_time[0].size() - 1;
    vector<int> order = Best_Sequence;
    int bestmakespan = INT_MAX;

    vector<vector<int>> e(n + 1, vector<int>(m + 1, 0)), e_2(n + 1, vector<int>(m + 1, 0));
    vector<vector<int>> f(n + 1, vector<int>(m + 2, 0)), f_2(n + 1, vector<int>(m + 2, 0));

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
vector<int> Population_Initialization(const int lambda, const int k, const vector<vector<int> > &processing_time,
                                      vector<int> indice) {
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
    return NEH_PI(lambda, n, order, processing_time); // 使用 NEH_PI 算法优化顺序
}


vector<vector<int> > ALG1(int N0, int lambda, int x) {
    // 初始化最佳顺序
    vector<int> Best_sequence = Population_Initialization(lambda, 1, globalData.processing_time, globalData.indice);

    for (int i = 2; i <= x; ++i) // 生成 x 种顺序并选择最佳
    {
        vector<int> sequence = Population_Initialization(lambda, i, globalData.processing_time, globalData.indice);
        if (sequence[0] < Best_sequence[0])
            Best_sequence = sequence; //选取最优
    }
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
    return globalData.POP;
}
