//
// Created by DELL on 24-9-9.
//

#include "Alg1.h"
#include "Utils.h"
#include "GlobalData.h"
#include "IO.h"
//#define IO_SHOW_PROCESSING_DATA
using namespace std;

//后lambda个job向前插入，评估makespan
vector<int> insert_lambda(int lambda, vector<int> &pi_prime, vector<vector<int> > d) {
    vector<vector<int> > processing_time = globalData.processing_time;
    int n = globalData.n;
    int m = globalData.m;

    lambda = min(lambda, n - 1); //防止lambda太大

    vector<int> order(n - lambda + 1, 0);
    copy(pi_prime.begin() + 1, pi_prime.begin() + n - lambda + 1, order.begin() + 1);

    vector<vector<int> > e_2(n + 1, vector<int>(m + 1, 0));
    copy(d.begin() + 1, d.begin() + n - lambda + 1, e_2.begin() + 1); //得到初始的e''
    vector<vector<int> > f_2(n + 1, vector<int>(m + 2, 0));
    Utils::calculate_tail_time(f_2, n - lambda, 1, order, processing_time); //得到初始的f''

#ifdef IO_SHOW_PROCESSING_DATA
    cout << "Init e'':" << endl;
    for (auto ele: e_2) {
        for (auto element: ele) {
            cout << element << " ";
        }
        cout << endl;
    }
    cout << "Init f'':" << endl;
    for (auto ele: f_2) {
        for (auto element: ele) {
            cout << element << " ";
        }
        cout << endl;
    }
#endif

    //对所有待插入job
    for (int i = n - lambda + 1; i <= n; ++i) {
        int job = pi_prime[i];
        int best_index;
        int bestmakespan = INT_MAX;
        pair<int, int> ans = Utils::neighbor_insertion(i - 1, job, e_2, f_2, processing_time);
        best_index = ans.first;
        bestmakespan = ans.second;
#ifdef IO_SHOW_PROCESSING_DATA
        cout << "best index: " << best_index << ", best makespan = " << bestmakespan << endl;
#endif
        order.insert(order.begin() + best_index, job); //更新order
        Utils::calculate_departure_time(e_2, best_index, i, order, processing_time); //更新e''
        //新f''的[best_index+1,i]用原f''的[best_index,i-1]
        copy(f_2.begin() + best_index, f_2.begin() + i, f_2.begin() + best_index + 1);
        Utils::calculate_tail_time(f_2, best_index, 1, order, processing_time); //更新f''
        if (i == n) order[0] = bestmakespan;
    }
    //int true_makespan = Utils::calculate(order,processing_time);
    //cout<<"true_makespan = "<<true_makespan<<endl;
#ifdef IO_SHOW_PROCESSING_DATA
    cout<<"one order:"<<endl;
    for(int i=0;i<order.size();i++) {
        cout << order[i] << " ";
    }
    cout << endl;
#endif
    return order;
}

// 生成一个序列
vector<int> generate_one(const int lambda, const int k, vector<int> indice) {
    int n = globalData.n;
    vector<int> order(2, 0); // 初始化顺序

    vector<vector<int> > processing_time = globalData.processing_time;
    int m = globalData.m;
    vector<vector<int> > d(n + 1, vector<int>(m + 1, 0));

    order[1] = indice[k]; //初始化

    // 计算d[1][_]
    d[1][0] = 0;
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
#ifdef IO_SHOW_PROCESSING_DATA
    cout << "The order of pi'" << endl;
    for (auto e: order) {
        cout << e << " ";
    }
    cout << endl;

    cout << "The departure time:" << endl;
    for (int i = 1; i <= n; i++) {
        cout << "job" << i << ": ";
        for (int j = 1; j <= m; j++) {
            cout << d[i][j] << " ";
        }
        cout << endl;
    }
#endif

    return insert_lambda(lambda, order, d); // 后lambda个job依次插入前面的位置
}

vector<vector<int> > ALG1(int N0, int lambda, int x) {
    // 初始化最佳顺序
    int min_makespan = inf;
    vector<int> best_sequence;
    for (int i = 1; i <= x; ++i) // 生成 x 种顺序并选择最佳
    {
        vector<int> sequence = generate_one(lambda, i, globalData.indice);
        if (sequence[0] < min_makespan) {
            best_sequence = sequence; //选取最优
            min_makespan = sequence[0];
        }
    }
    globalData.POP.push_back(best_sequence); //加入种群

#ifdef IO_SHOW_PROCESSING_DATA
    // 输出最佳顺序及其最优时间跨度
    cout << "sequence:";
    for (int i = 1; i <= globalData.n; ++i)
        cout << " " << best_sequence[i];
    cout << endl << "minmakespan:" << best_sequence[0] << endl;
#endif
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

    return globalData.POP;
}
