//
// Created by DELL on 24-11-20.
//

#ifndef GLOBALDATA_H
#define GLOBALDATA_H

#include <vector>
#include <iostream>
#include<algorithm>
#include<numeric>
using namespace std;

class GlobalData {
public:
    // 全局变量
    int n; // 组件数量
    int m; // 机器数量
    vector<vector<int> > processing_time; // 处理时间矩阵
    vector<int> total_processing_time; // 每个组件的总处理时间
    vector<int> indice; // 组件索引

    vector<vector<int> > POP; //种群
    vector<vector<int> > POP2;
    vector<int> best_seq;
    int bestmakespan;

    int k_max; //迭代总次数
    int P_max; //10
    double pls; //概率
    // 构造函数
    GlobalData() : n(0), m(0), k_max(50), P_max(10), pls(0.15) {//p_max=10
    }

    ~GlobalData() = default;

    // 设置函数
    void setData();
};

// 全局数据对象
extern GlobalData globalData;

#endif //GLOBALDATA_H
