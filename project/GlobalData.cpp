//
// Created by DELL on 24-11-20.
//
#include "GlobalData.h"
#include "Utils.h"
using namespace std;

void GlobalData::setData() {
    std::cout << "The number of components: ";
    std::cin >> n;
    std::cout << "The number of machines: ";
    std::cin >> m;
    processing_time.assign(n + 1, std::vector<int>(m + 1, 0));
    total_processing_time.assign(n + 1, 0);
    indice.assign(n + 1, 0);
    iota(indice.begin(), indice.end(), 0); // 初始化组件索引0~n
    printf("\nThe processing time of each component on different machines:\n");
    for (int i = 1; i <= m; ++i)
        for (int j = 1; j <= n; ++j) {
            cin >> processing_time[j][i]; // 输入处理时间
            total_processing_time[j] += processing_time[j][i]; // 更新总处理时间
        }
    Utils::sort_by_tot_processing_time(indice,total_processing_time);
    // for(auto e: indice) {
    //     std::cout << e << " ";
    // }
}

GlobalData::GlobalData(const GlobalData &gDtmp):
    n(gDtmp.n), m(gDtmp.m),
    processing_time(gDtmp.processing_time),
    total_processing_time(gDtmp.total_processing_time),
    indice(gDtmp.indice), POP(gDtmp.POP),
    POP2(gDtmp.POP2), best_seq(gDtmp.best_seq),
    bestmakespan(gDtmp.bestmakespan),
    k_max(gDtmp.k_max), P_max(gDtmp.P_max),
    pls(gDtmp.pls){
    _dataMemoryPtr = new _dataMemory(n, m, processing_time, total_processing_time,
                                     indice, POP, POP2, best_seq, bestmakespan,
                                     k_max, P_max, pls);
    if(_dataMemoryPtr == nullptr)
        throw std::runtime_error("_dataMemoryPtr: Allocation error!");
}

//GlobalData& GlobalData::operator=(const GlobalData &gDtmp) {
//    if(this == &gDtmp)
//        return *this;
//    this->n = gDtmp.n;
//    this->m = gDtmp.m;
//    this->indice = gDtmp.indice;
//    this->total_processing_time = gDtmp.total_processing_time;
//    this->processing_time = gDtmp.processing_time;
//
//    this->POP = gDtmp.POP;
//    this->POP2 = gDtmp.POP2;
//    this->best_seq = gDtmp.best_seq;
//    this->bestmakespan = gDtmp.bestmakespan;
//    this->k_max = gDtmp.k_max;
//    this->P_max = gDtmp.P_max;
//    this->pls = gDtmp.pls;
//
//    return *this;
//}

void GlobalData::resetData() {
    n = this->_dataMemoryPtr->n;
    m = this->_dataMemoryPtr->m;
    processing_time = this->_dataMemoryPtr->processing_time;
    total_processing_time = this->_dataMemoryPtr->total_processing_time;
    indice = this->_dataMemoryPtr->indice;
    POP = this->_dataMemoryPtr->POP;
    POP2 = this->_dataMemoryPtr->POP2;
    best_seq = this->_dataMemoryPtr->best_seq;
    bestmakespan = this->_dataMemoryPtr->bestmakespan;
    k_max = this->_dataMemoryPtr->k_max;
    P_max = this->_dataMemoryPtr->P_max;
    pls = this->_dataMemoryPtr->pls;

}

// 定义全局数据对象
GlobalData globalData;
