//
// Created by DELL on 24-11-20.
//
#include "Utils.h"

#include <chrono>

#include "GlobalData.h"
using namespace std;

void Utils::remove_non_improving_moves(const vector<vector<int>> &e, const vector<vector<int>> &f, const int c_max, vector<vector<int>> &V, const vector<int> order, const vector<vector<int>> processing_time) // 利用正反块去除掉没有意义的插入操作
{
    int m = processing_time[0].size() - 1;
    int n = order.size() - 1;
    vector<vector<int>> B(m + 1), AB(m + 1);
    for (int j = 1; j < n; ++j)
        for (int k = m; k > 0; --k)
        {
            if (e[j][k] + f[j + 1][k] == c_max)
            {
                if (e[j][k] == e[j][k - 1] + processing_time[j][k])
                {
                    if (B[k].empty())
                        B[k] = {j, k};
                    else
                        B[k].push_back(j + 1);
                }
                else if (e[j][k] > e[j][k - 1] + processing_time[j][k])
                {
                    if (AB[k].empty())
                        AB[k] = {j, j + 1};
                    else
                    {
                        AB[k] = AB[k + 1];
                        AB[k].push_back(j + 1);
                        AB[k + 1].clear();
                    }
                    if (k == 1)
                    {
                        int gk = AB[k][0];
                        int hk = AB[k].back();

                        for (int i = gk; i <= hk; ++i)
                            for (int q = gk; q <= hk; ++q)
                            {
                                if (V[i][q] && (i == gk || q != gk))
                                    V[i][q] = 0;
                            }

                        AB[k].clear();
                    }
                }
            }

            if (e[j][k] + f[j + 1][k] < c_max)
            {
                if (!B[k].empty())
                {
                    int gk = B[k][0];
                    int hk = B[k].back();

                    for (int i = gk; i <= hk; ++i)
                        for (int q = gk; q <= hk; ++q)
                        {
                            if (V[i][q] && (i == gk || q != gk))
                                V[i][q] = 0;
                        }
                    B[k].clear();
                }

                if (!AB[k + 1].empty())
                {
                    int gk = AB[k + 1][0];
                    int hk = AB[k + 1].back();

                    for (int i = gk; i <= hk; ++i)
                        for (int q = gk; q <= hk; ++q)
                        {
                            if (V[i][q] && (i == gk || q != gk))
                                V[i][q] = 0;
                        }

                    AB[k + 1].clear();
                }
            }
        }
}

//从前向后，更新第begin个到第end个job的离开时间e
void Utils::calculate_departure_time(vector<vector<int>> &e, const int begin, const int end, const vector<int> &order, const vector<vector<int>> &processing_time)
{
    int m = processing_time[0].size() - 1;
    for (int i = begin; i <= end; ++i)
    {
        if (i == 1)
        {
            e[1][0] = 0;
            for (int j = 1; j <= m - 1; ++j)
                e[1][j] = e[1][j - 1] + processing_time[order[1]][j];
        }
        else
        {
            e[i][0] = e[i - 1][1];
            for (int j = 1; j <= m - 1; ++j)
                e[i][j] = max(e[i][j - 1] + processing_time[order[i]][j], e[i - 1][j + 1]);
        }
        e[i][m] = e[i][m - 1] + processing_time[order[i]][m];
    }
}

//从后向前，更新第begin个到第end个job的tail time f
void Utils::calculate_tail_time(vector<vector<int>> &f, const int begin, const int end, const vector<int> &order, const vector<vector<int>> &processing_time)
{
    int n = order.size() - 1;
    int m = processing_time[0].size() - 1;
    for (int i = min(n, begin); i >= end; --i)//
    {
        if (i == n)
        {
            f[n][m+1]=0;
            for (int j = m; j >= 2; --j)
                f[n][j] = f[n][j + 1] + processing_time[order[n]][j];
        }
        else
        {
            f[i][m+1] = f[i+1][m];
            for (int j = m; j >= 2; --j)
                f[i][j] = max(f[i][j + 1] + processing_time[order[i]][j], f[i + 1][j - 1]);

        }
        f[i][1] = f[i][2] + processing_time[order[i]][1];
    }
}

//计算完工时间,old
int Utils::calculate(const vector<int> &order, const vector<vector<int> > &processing_time) {
    int n = order.size() - 1; // 任务数量 (job count)
    int m = processing_time[0].size() - 1; // 机器数量 (machine count)

    // 定义 dπ(i,j)，即公式中的开始时间矩阵
    vector<vector<int> > d(n + 1, vector<int>(m + 1, 0));

    // 按照图片中的公式逐步计算
    // 初始化公式 (1)
    d[1][0] = 0;

    // 计算第一行 (组件 1)，公式 (2)
    for (int j = 1; j <= m; ++j) {
        d[1][j] = d[1][j - 1] + processing_time[order[1]][j];
    }

    // 计算剩余任务的时间矩阵
    for (int i = 2; i <= n; ++i) {
        // 遍历每个任务 (从第2个任务开始)
        // 初始化公式 (3)
        d[i][0] = d[i - 1][1];

        // 公式 (4)
        for (int j = 1; j <= m - 1; ++j) {
            d[i][j] = max(d[i][j - 1] + processing_time[order[i]][j],
                          d[i - 1][j + 1]);
        }
        d[i][m] = d[i][m - 1] + processing_time[order[i]][m];
    }

    // 返回最后一个任务在最后一台机器的完成时间 (Cmax)
    return d[n][m];
}

// 计算阻塞和空闲时间之和
vector<vector<int> > Utils::calculate_v(int &v, const int i, const vector<int> &order, vector<vector<int> > d,
                                        const vector<vector<int> > &processing_time) {
    int m = processing_time[0].size() - 1; // 机器数量

    d[i][0] = d[i - 1][1];

    for (int j = 1; j <= m - 1; ++j) {
        d[i][j] = max(d[i][j - 1] + processing_time[order[i]][j],
                      d[i - 1][j + 1]);
        v += d[i][j] - d[i - 1][j] - processing_time[order[i]][j];
    }
    d[i][m] = d[i][m - 1] + processing_time[order[i]][m];
    v += d[i][m] - d[i - 1][m] - processing_time[order[i]][m];
    return d;
}

//随机序列生成
vector<int> Utils::random_init(int n) {
    vector<int> seq(n + 1);
    for (int i = 0; i <= n; ++i) {
        seq[i] = i;
    }
    random_device rd;
    mt19937 g(rd());
    shuffle(seq.begin() + 1, seq.end(), g);

    return seq;
}

// 检查序列是否已经在 POP 中存在
bool Utils::is_unique(const vector<int> &seq, const vector<vector<int> > &POP) {
    for (const auto &existing_seq: POP) {
        if (seq == existing_seq) {
            return false;
        }
    }
    return true;
}

//按总加工时间升序排列
void Utils::sort_by_tot_processing_time(vector<int> &pi_R, const vector<int> &total_processing_time) {
    // 使用 Lambda 表达式根据 total_processing_time 进行排序
    std::sort(pi_R.begin(), pi_R.end(), [&total_processing_time](int a, int b) {
        return total_processing_time[a] < total_processing_time[b];
    });
}

vector<int> Utils::findBestpi(const vector<vector<int> > &pop) {
    vector<int> pi_best;
    int best = INT_MAX;
    for (int i = 0; i < pop.size(); ++i) {
        if (pop[i][0] < best) {
            best = pop[i][0];
            pi_best = pop[i];
        }
    }
    return pi_best;
}

void Utils::sortAllpi(vector<vector<int> > &pop) {
    // 使用 Lambda 表达式进行排序
    std::sort(pop.begin(), pop.end(), [](const std::vector<int> &a, const std::vector<int> &b) {
        return a[0] < b[0];
    });
}

void Utils::shuffleArray(std::vector<int> &array) {
    //Fisher–Yates shuffle method
    // 使用当前时间作为随机数生成器的种子
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine rng(seed);

    for (int i = array.size() - 1; i > 0; --i) {
        // 生成一个 [1, i] 之间的随机整数
        std::uniform_int_distribution<int> dist(1, i);
        int j = dist(rng);

        // 交换 array[i] 和 array[j]
        std::swap(array[i], array[j]);
    }
}

void Utils::print_pop(const vector<vector<int> > pop) {
    cout<<"==================== pop now: ===================="<<endl;
    for (int i = 0; i < pop.size(); ++i) {
        for(int j = 0; j < pop[i].size(); ++j) {
            cout << pop[i][j] << " ";
        }
        cout << endl;
    }
    cout<<"==================== end of pop ===================="<<endl;
}
void Utils::print_pi(vector<int> order) {
    cout<<"=================== pi order: =================== "<<endl;
    for(int i = 0; i < order.size(); ++i) {
        cout << order[i] << " ";
    }
    cout<<endl;
    cout<<"==================== end of pi order =================="<<endl;
}

/*
 *  @R: 运行次数
 *  @C_min: 此算例下的最优解的完工时间
 *  @C_i: 此算例下某算法运行R次得到的序列
 *  计算单个的算例的ARPD
 *  本ARPD的计算将原公式化简推导后得到
 */
double Utils::ARPD(const int &R, const int &C_min, const vector<int> &C_i)
{
    if (R <= 0)
        throw std::runtime_error("ARPD error: R <= 0!");
    if (C_min <= 0)
        throw std::runtime_error("ARPD error: C_min <= 0!");
    if (R != C_i.size())
        throw std::runtime_error("ARPD error: R!= C_i.size()!");

    int C_sum = std::accumulate(C_i.begin(), C_i.end(), 0);
    double ans = (static_cast<double>(100 * C_sum) / static_cast<double>(R * C_min)) - 100.0;
    return ans;
}

//返回位置、makespan
pair<int,int> Utils::neighbor_insertion(int length, int job, vector<vector<int> > e_2, vector<vector<int> > f_2, vector<vector<int> >processing_time) {
    int m = globalData.m;
    int best_index = -1;
    int bestmakespan = INT_MAX;
    for (int q = 1; q <= length+1; q++) {
        //尝试所有位置
        //cout << "Inserting job " << i << " into" << " position " << q << endl;
        vector<int> e_prime_q(m + 1, 0);
        int c_max = 0;
        //计算出e'[q][k]
        if (q == 1) {
            e_prime_q[0] = 0;
            for (int j = 1; j <= m - 1; ++j)
                e_prime_q[j] = e_prime_q[j - 1] + processing_time[job][j];
        } else {
            e_prime_q[0] = e_2[q - 1][1];
            for (int j = 1; j <= m - 1; ++j)
                e_prime_q[j] = max(e_prime_q[j - 1] + processing_time[job][j], e_2[q - 1][j + 1]);
        }
        e_prime_q[m] = e_prime_q[m - 1] + processing_time[job][m];
        //计算出makespan
        for (int k = 1; k <= m; ++k) {
            c_max = max(c_max, e_prime_q[k] + f_2[q][k]);
            if(c_max >= bestmakespan) break;//剪枝
        }

        //cout << "position " << q << ": ,c_max = " << c_max << endl;
        //更新
        if (c_max < bestmakespan) {
            bestmakespan = c_max;
            best_index = q;
        }
    }
    return make_pair(best_index, bestmakespan);
}