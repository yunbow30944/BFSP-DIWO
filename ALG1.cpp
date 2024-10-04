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

// 计算阻塞和空闲时间之和，可优化
int calculate_v(const vector<int>& order, const vector<vector<int>>& processing_time)
{
    int n = order.size() - 1; // 组件数量
    int m = processing_time[0].size() - 1; // 机器数量
    vector<int> finish_time(m + 1, 0); // 每台机器的完成时间
    int v=0;
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

        if(i==n) v += temp - finish_time[j]-processing_time[i][j]; // 更新阻塞和空闲时间之和
        finish_time[j] = temp; // 更新当前机器的完成时间
    }
    return v; 
}
//计算完工时间
int calculate(const vector<int>& order, const vector<vector<int>>& processing_time)
{
    int n = order.size() - 1; // 组件数量
    int m = processing_time[0].size() - 1; // 机器数量
    vector<int> finish_time(m + 1, 0); // 每台机器的完成时间
    for(int i = 1; i <= n; ++i) // 遍历每个组件
        for(int j = 1; j <= m; ++j) // 遍历每台机器
        {
            if(i == 1)
                finish_time[j] = finish_time[j - 1] + processing_time[order[i]][j]; // 第一个组件
            else if(j != m)
                finish_time[j] = max(finish_time[j - 1], finish_time[j + 1] - processing_time[order[i - 1]][j + 1]) + processing_time[order[i]][j]; // 中间机器
            else
                finish_time[j] = max(finish_time[j - 1], finish_time[j]) + processing_time[order[i]][j]; // 最后一台机器
        }
    return finish_time[m]; // 返回最后一台机器的完成时间
}

// NEH_PI 算法来优化顺序
vector<int> NEH_PI(const int lambda, const int n, vector<int>& Best_Sequence, const vector<vector<int>>& processing_time)
{
    vector<int> order(n-lambda+1); // 当前顺序

    copy(Best_Sequence.begin()+1, Best_Sequence.begin() + n-lambda+1, order.begin()+1);

    for(int i = n - lambda + 1; i <= n; ++i) // 处理后 lambda 个组件
    {
        int indice = Best_Sequence[i]; // 取出当前组件
        int index=0;
        int bestmakespan = INT_MAX;
        for(int j = 1; j <= i; ++j) // 插入位置的尝试
        {
            vector<int>temp_order=order;
            temp_order.insert(temp_order.begin() + j, indice); // 将组件插入到位置 j
            int makespan = calculate(temp_order, processing_time); // 计算插入后的处理时间
    
            if(makespan < bestmakespan) // 更新最优顺序
            {
                index=j;
                bestmakespan = makespan;
            }
        }
        order.insert(order.begin() + index, indice); // 将组件插入到位置 j
        order[0] = bestmakespan;
    }
    return order; // 返回最优顺序
}

// 初始化种群，生成一个初始的优良序列，需修改
vector<int> Population_Initialization(const int lambda, const int k, const vector<vector<int>> processing_time, vector<int> indice)
{
    int n = indice.size() - 1; // 组件数量
    vector<int> order(2, 0); // 初始化顺序

    order[1]=indice[k];//初始化

    indice.erase(indice.begin() + k); // 移除已插入的组件

    while(indice.size() > 1) // 直到所有组件都被插入
    {
        int min_v = INT_MAX; // 最小处理时间
        int best_indice = 0; // 最优组件
        for(int j = 1; j < indice.size(); ++j) // 尝试每个剩余的组件
        {
            order.push_back(indice[j]);
            int v = 0;
            v=calculate_v(order, processing_time); // 计算阻塞和空闲时间之和
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
    //测试order
    // for(auto e:order) {
    //     cout<<e<<" ";
    // }
    return NEH_PI(lambda, n, order, processing_time); // 使用 NEH_PI 算法优化顺序
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
unsigned long long factorial(int n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}
// 主函数
int main()
{
    int n, m, lambda, x,N0;
    printf("The number of components:");
    cin >> n; // 输入组件数量
    printf("The number of machines:");
    cin >> m; // 输入机器数量
    printf("lambda:(lambda<%d)",n);
    cin >> lambda; // 输入 lambda 值
    printf("x:(x<%d)",n);
    cin >> x; // 输入 x 值
    vector<vector<int>> processing_time(n + 1, vector<int>(m + 1, 0)); // 处理时间矩阵
    vector<int> total_processing_time(n + 1, 0); // 每个组件的总处理时间
    vector<int> indice(n + 1, 0); // 组件索引
    iota(indice.begin(), indice.end(), 0); // 初始化组件索引0~n-1
    vector<vector<int>>POP;//种群

    printf("\nThe processing time of each component on different machines:\n");
    for(int i = 1; i <= n; ++i)
    for(int j = 1; j <= m; ++j)
    {
        cin >> processing_time[i][j]; // 输入处理时间
        total_processing_time[i] += processing_time[i][j]; // 更新总处理时间
    }
    // 按总处理时间降序排序组件索引
    sort(indice.begin() + 1, indice.end(), [&total_processing_time](int a, int b){
        return total_processing_time[a] < total_processing_time[b];
    });
    //测试排序结果
    // for(auto e:indice) {
    //     cout<<e<<" ";
    // }
    // 初始化最佳顺序
    vector<int> Best_sequence = Population_Initialization(lambda, 1, processing_time, indice);
    for(auto e:Best_sequence) {
        cout<<e<<" ";
    }
    for(int i = 2; i <= x; ++i) // 生成 x 种顺序并选择最佳
    {
        vector<int> sequence = Population_Initialization(lambda, i, processing_time, indice);
        if(sequence[0] < Best_sequence[0])
            Best_sequence = sequence;//选取最优
    }
    POP.push_back(Best_sequence);//加入种群

    // 输出最佳顺序及其最优时间跨度
    cout << "sequence:" ;
    for(int i = 1; i <= n; ++i)
        cout << " " << Best_sequence[i];
    cout << endl << "minmakespan:" << Best_sequence[0] << endl;

    printf("N0:(N0<=%d)",factorial(n));//测试用
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
    return 0;
}
