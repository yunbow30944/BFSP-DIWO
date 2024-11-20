//
// Created by DELL on 24-11-20.
//
#include "Utils.h"
using namespace std;


//计算完工时间
int Utils::calculate(const vector<int>& order, const vector<vector<int>>& processing_time)
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
// 计算阻塞和空闲时间之和，可优化
int Utils::calculate_v(const vector<int>& order, const vector<vector<int>>& processing_time)
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
//随机序列生成
vector<int> Utils::random_init(int n) {
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
bool Utils::is_unique(const vector<int>& seq, const vector<vector<int>>& POP) {
    for (const auto& existing_seq : POP) {
        if (seq == existing_seq) {
            return false;
        }
    }
    return true;
}
