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
    // 全局变量
public:
    // 算例信息
    int n; // 组件数量
    int m; // 机器数量
    vector<vector<int> > processing_time; // 处理时间矩阵
    vector<int> total_processing_time; // 每个组件的总处理时间
    vector<int> indice; // 组件索引

    //运行时信息
    vector<vector<int> > POP; //种群
    vector<vector<int> > POP2;
    vector<int> best_seq;
    int bestmakespan;

    //超参数
    int lambda, x, P_max; //ALG1参数
    int S_MIN, S_MAX, SIGMA_MIN, SIGMA_MAX; //ALG2参数
    double pls; //ALG3参数
    int rho; // 时间

    //调参用
    inline const static vector<int> P_MAX_ARRAY = {10}; //10,20,30
    inline const static vector<int> S_MIN_ARRAY = {0}; //0,1,2
    inline const static vector<int> S_MAX_ARRAY = {7}; //5,7,9
    inline const static vector<int> SIGMA_MIN_ARRAY = {5}; //4,5,6
    inline const static vector<int> SIGMA_MAX_ARRAY = {10}; //8,9,10
    inline const static vector<double> PLS_ARRAY = {0.15}; //0.15, 0.25, 0.35
    inline const static vector<int> RHO_ARRAY = {90}; //{30, 60, 90};

    // 无参构造函数，只初始化超参数
    GlobalData() : n(0), m(0), lambda(25), x(5), P_max(10), S_MIN(0), S_MAX(7), SIGMA_MIN(5), SIGMA_MAX(10), pls(0.15),
                   rho(90) {
    }

    // 有参构造函数，将算例信息保存
    GlobalData(const GlobalData &gDtmp): n(gDtmp.n), m(gDtmp.m), processing_time(gDtmp.processing_time),
                                         total_processing_time(gDtmp.total_processing_time), indice(gDtmp.indice) {
        _dataMemoryPtr = new _dataMemory(n, m, processing_time, total_processing_time, indice);
    }

    ~GlobalData() {
        if (_dataMemoryPtr) {
            delete _dataMemoryPtr;
            _dataMemoryPtr = nullptr;
        }
    }

    // 设置函数
    void setData();

    // 用于恢复初值的函数
    void resetData();

private:
    // 用于存储算例信息的类
    class _dataMemory {
    public:
        const int n; // 组件数量
        const int m; // 机器数量
        const vector<vector<int> > processing_time; // 处理时间矩阵
        const vector<int> total_processing_time; // 每个组件的总处理时间
        const vector<int> indice; // 组件索引

    public:
        // 构造函数
        _dataMemory(const int &n, const int &m, const vector<vector<int> > &processing_time,
                    const vector<int> &total_processing_time, const vector<int> &indice): n(n), m(m),
            processing_time(processing_time), total_processing_time(total_processing_time), indice(indice) {
        }
    };

private:
    _dataMemory *_dataMemoryPtr;
};

// 全局数据对象
extern GlobalData globalData;

#endif //GLOBALDATA_H
