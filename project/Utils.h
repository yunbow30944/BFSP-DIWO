//
// Created by DELL on 24-11-20.
//

#ifndef UTILS_H
#define UTILS_H


#include <vector>
#include <iostream>
#include<climits>
#include<random>
#include <vector>
#include<algorithm>
using namespace std;

class Utils {
public:
    // 工具函数示例：打印二维向量
    static void print2DVector(const std::vector<std::vector<int>>& vec);
    static int calculate(const vector<int>& order, const vector<vector<int>>& processing_time);
    static int calculate_v(const vector<int>& order, const vector<vector<int>>& processing_time);
    static vector<int> random_init(int n);
    static bool is_unique(const vector<int>& seq, const vector<vector<int>>& POP);
};

#endif //UTILS_H
