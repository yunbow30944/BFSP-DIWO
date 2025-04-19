//
// Created by DELL on 24-11-20.
//

#ifndef GLOBALDATA_H
#define GLOBALDATA_H

#include <vector>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <memory>
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

    int lambda;
    int rho;

    inline const static vector<int> P_MAX_ARRAY = {10, 20, 30};
    inline const static vector<int> S_MIN_ARRAY = {0, 1, 2};
    inline const static vector<int> S_MAX_ARRAY = {5, 7, 9};
    inline const static vector<int> SIGMA_MIN_ARRAY = {4, 5, 6};
    inline const static vector<int> SIGMA_MAX_ARRAY = {8, 9, 10};
    inline const static vector<double> PLS_ARRAY = {0.15, 0.25, 0.35};
    inline const static vector<int> RHO_ARRAY = {90}; //{30, 60, 90};
    
    inline static int S_MIN;
    inline static int S_MAX;
    inline static int SIGMA_MIN;
    inline static int SIGMA_MAX;

    // 构造函数
    GlobalData() : n(0), m(0), k_max(50), P_max(10), pls(0.15), lambda(25), rho(10), 
        _dataMemoryPtr(new _dataMemory(n, m, processing_time, total_processing_time,
                                      indice, POP, POP2, best_seq, bestmakespan,
                                      k_max, P_max, pls,
                                      lambda, rho
                                      )){//p_max=10
        if(_dataMemoryPtr == nullptr)
            throw std::domain_error("_dataMemoryPtr allocation failed!");
    }

    GlobalData(const GlobalData& gDtmp);

//    GlobalData& operator=(const GlobalData& gDtmp);

    ~GlobalData() {
        if(_dataMemoryPtr){
            delete _dataMemoryPtr;
            _dataMemoryPtr = nullptr;
        }
    }

    // 设置函数
    void setData();

    // 用于恢复初值的函数
    void resetData();

private:
    // 用于存储初始值的类
    class _dataMemory{
    public:
        const int n; // 组件数量
        const int m; // 机器数量
        const vector<vector<int> > processing_time; // 处理时间矩阵
        const vector<int> total_processing_time; // 每个组件的总处理时间
        const vector<int> indice; // 组件索引

        const vector<vector<int> > POP; //种群
        const vector<vector<int> > POP2;
        const vector<int> best_seq;
        const int bestmakespan;

        const int k_max; //迭代总次数
        const int P_max; //10
        const double pls; //概率

        const int lambda;
        const int rho;

    public:
        _dataMemory(const int &n, const int &m, const vector<vector<int>> &processing_time,
            const vector<int> &total_processing_time, const vector<int> & indice,
            const vector<vector<int>> &POP, const vector<vector<int>> &POP2,
            const vector<int> &best_seq, const int &bestmakespan, const int &k_max,
            const int &P_max, const double &pls,
            const int &lambda, const int &rho
            ):
                n(n), m(m),
                processing_time(processing_time),
                total_processing_time(total_processing_time),
                indice(indice), POP(POP),
                POP2(POP2), best_seq(best_seq),
                bestmakespan(bestmakespan),
                k_max(k_max), P_max(P_max),
                pls(pls),
                lambda(lambda), rho(rho)
                {}
    };

private:
    _dataMemory* _dataMemoryPtr;
};

// 全局数据对象
extern GlobalData globalData;

#endif //GLOBALDATA_H
