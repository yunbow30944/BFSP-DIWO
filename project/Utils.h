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
    static int calculate(const vector<int> &order, const vector<vector<int> > &processing_time);

    static vector<vector<int> > calculate_v(int &v, int i, const vector<int> &order, vector<vector<int> > d,
                                            const vector<vector<int> > &processing_time);

    static vector<int> random_init(int n);

    static bool is_unique(const vector<int> &seq, const vector<vector<int> > &POP);

    static void sort_by_tot_processing_time(vector<int> &pi_R, const vector<int> &total_processing_time);

    static vector<int> findBestpi(const vector<vector<int> > &pop);

    static void sortAllpi(vector<vector<int> > &pop);
};

#endif //UTILS_H
