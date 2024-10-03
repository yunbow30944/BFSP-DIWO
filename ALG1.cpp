//
// Created by DELL on 24-9-9.
//
#include<iostream>
#include<vector>
#include<algorithm>
#include<numeric>
#include<climits>
#include<random>
using namespace std;

// 计算给定顺序下的总处理时间和机器的最晚完成时间
int calculate_v(int& v, const vector<int>& order, const vector<vector<int>>& processing_time)
{
    int n = order.size() - 1; // 组件数量
    int m = processing_time[0].size() - 1; // 机器数量
    vector<int> finish_time(m + 1, 0); // 每台机器的完成时间
    for(int i = 1; i <= n; ++i) // 遍历每个组件
    for(int j = 1; j <= m; ++j) // 遍历每台机器
    {
        int temp; // 当前处理时间
        if(i == 1)
            temp = finish_time[j - 1] + processing_time[order[i]][j]; // 第一个组件
        else if(j != m)
            temp = max(finish_time[j - 1], finish_time[j + 1] - processing_time[order[i - 1]][j + 1]) + processing_time[order[i]][j]; // 中间机器
        else
            temp = max(finish_time[j - 1], finish_time[j]) + processing_time[order[i]][j]; // 最后一台机器

        v += temp - finish_time[j]; // 更新总处理时间
        finish_time[j] = temp; // 更新当前机器的完成时间
    }
    return finish_time[m]; // 返回最后一台机器的完成时间
}
int calculate(const vector<int>& order, const vector<vector<int>>& processing_time)
{
    int n = order.size() - 1; // 组件数量
    int m = processing_time[0].size() - 1; // 机器数量
    vector<int> finish_time(m + 1, 0); // 每台机器的完成时间
    for(int i = 1; i <= n; ++i) // 遍历每个组件
        for(int j = 1; j <= m; ++j) // 遍历每台机器
        {
            int temp; // 当前处理时间
            if(i == 1)
                temp = finish_time[j - 1] + processing_time[order[i]][j]; // 第一个组件
            else if(j != m)
                temp = max(finish_time[j - 1], finish_time[j + 1] - processing_time[order[i - 1]][j + 1]) + processing_time[order[i]][j]; // 中间机器
            else
                temp = max(finish_time[j - 1], finish_time[j]) + processing_time[order[i]][j]; // 最后一台机器
            finish_time[j] = temp; // 更新当前机器的完成时间
        }
    return finish_time[m]; // 返回最后一台机器的完成时间
}

// NEH_PI 算法来优化顺序
vector<int> NEH_PI(const int lambada, const int n, vector<int>& Best_Sequence, const vector<vector<int>>& processing_time)
{
    vector<int> order; // 当前顺序
    int best_v = 0; // 最优总处理时间
    int bestmakespan = calculate_v(best_v, Best_Sequence, processing_time); // 当前最优的时间跨度
    for(int i = n - lambada + 1; i <= n; ++i) // 处理最近的 lambada 个组件
    {
        order = Best_Sequence; // 复制当前最优顺序
        int indice = Best_Sequence[i]; // 取出当前组件
        order.erase(order.begin() + i); // 从顺序中移除组件
        for(int j = 1; j < i; ++j) // 插入位置的尝试
        {
            order.insert(order.begin() + j, indice); // 将组件插入到位置 j
            int v = 0;
            int makespan = calculate_v(v, order, processing_time); // 计算插入后的处理时间
            if(v < best_v) // 更新最优顺序
            {
                Best_Sequence = order;
                best_v = v;
                bestmakespan = makespan;
            }
        }
    }
    Best_Sequence[0] = bestmakespan; // 将最优时间跨度存入结果
    return Best_Sequence; // 返回最优顺序
}

// 初始化种群，生成一个初始的优良序列
vector<int> Population_Initialization(const int lambada, const int k, const vector<vector<int>> processing_time, vector<int> indice)
{
    int n = indice.size() - 1; // 组件数量
    vector<int> order(1, 0); // 初始化顺序
    int i = 1;
    order.insert(order.begin() + i, indice[k]); // 插入初始组件
    indice.erase(indice.begin() + k); // 移除已插入的组件
    while(indice.size() > 1) // 直到所有组件都被插入
    {
        ++i;
        int min_v = INT_MAX; // 最小处理时间
        int best_indice = 0; // 最优组件
        for(int j = 1; j < indice.size(); ++j) // 尝试每个剩余的组件
        {
            order.push_back(indice[j]);
            int v = 0;
            calculate_v(v, order, processing_time); // 计算当前顺序的处理时间
            if(min_v > v) // 更新最优组件
            {
                best_indice = j;
                min_v = v;
            }
            order.pop_back(); // 回退
        }
        order.push_back(indice[best_indice]); // 插入最优组件
        indice.erase(indice.begin() + best_indice); // 移除已插入的组件
    }
    return NEH_PI(lambada, n, order, processing_time); // 使用 NEH_PI 算法优化顺序
}

//随机序列生成
vector<int> random_init(int n) {
    vector<int> seq(n+1);
    for (int i = 0; i <= n; ++i) {
        seq[i] = i;
    }
    random_device rd;
    mt19937 g(rd());
    shuffle(seq.begin()+1, seq.end(), g);

    return seq;
}

// 检查序列是否已经在 POP 中存在
bool is_unique(const vector<int>& seq, const vector<vector<int>>& POP) {
    for (const auto& existing_seq : POP) {
        if (seq == existing_seq) {
            return false;
        }
    }
    return true;
}

// 主函数
int main()
{
    int n, m, lambada, x,N0;
    printf("The number of components:");
    cin >> n; // 输入组件数量
    printf("The number of machines:");
    cin >> m; // 输入机器数量
    printf("lambada:");
    cin >> lambada; // 输入 lambada 值
    printf("x:");
    cin >> x; // 输入 x 值
    vector<vector<int>> processing_time(n + 1, vector<int>(m + 1, 0)); // 处理时间矩阵
    vector<int> total_processing_time(n + 1, 0); // 每个组件的总处理时间
    vector<int> indice(n + 1, 0); // 组件索引
    iota(indice.begin(), indice.end(), 0); // 初始化组件索引0~n-1
    vector<vector<int>>POP;//种群

    printf("The processing time of each component on different machines:\n");
    for(int i = 1; i <= n; ++i)
    for(int j = 1; j <= m; ++j)
    {
        cin >> processing_time[i][j]; // 输入处理时间
        total_processing_time[i] += processing_time[i][j]; // 更新总处理时间
    }
    // 按总处理时间降序排序组件索引？
    sort(indice.begin() + 1, indice.end(), [&total_processing_time](int a, int b){
        return total_processing_time[a] < total_processing_time[b];//？
    });
    //测试排序结果
    // for(auto e:indice) {
    //     cout<<e<<" ";
    // }
    // 初始化最佳顺序
    vector<int> Best_sequence = Population_Initialization(lambada, 1, processing_time, indice);
    for(int i = 2; i <= x; ++i) // 生成 x 种顺序并选择最佳
    {
        vector<int> sequence = Population_Initialization(lambada, i, processing_time, indice);
        if(sequence[0] < Best_sequence[0])
            Best_sequence = sequence;//选取最优
    }
    POP.push_back(Best_sequence);//加入种群

    // 输出最佳顺序及其最优时间跨度
    cout << "sequence:" ;
    for(int i = 1; i <= n; ++i)
        cout << " " << Best_sequence[i];
    cout << endl << "minmakespan:" << Best_sequence[0] << endl;

    cout<<"N0:";
    cin>>N0;
    /*完成种群初始化*/
    for(int i=2;i<=N0;i++) {
        vector<int>seq;
        bool unique=true;
        do {
            seq = random_init(n); // 生成随机排列
            seq[0]=calculate(seq,processing_time);//计算时间
            unique = is_unique(seq, POP); // 检查是否唯一
        } while (!unique);

        POP.push_back(seq); // 加入POP
    }
    //测试POP
    for(auto ele:POP) {
        for(auto e:ele) {
            cout<<e<<" ";
        }
        cout<<endl;
    }

}