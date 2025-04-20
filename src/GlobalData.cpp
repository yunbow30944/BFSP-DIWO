//
// Created by DELL on 24-11-20.
//
#include "GlobalData.h"
#include "Utils.h"
//#define IO_SHOW_PROCESSING_DATA
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
#ifdef IO_SHOW_PROCESSING_DATA
    cout<<"The initial order :\n";
    for(auto e: indice) {
        std::cout << e << " ";
    }
    cout<<endl;
#endif
}
GlobalData::GlobalData(const GlobalData &gDtmp):
    n(gDtmp.n), m(gDtmp.m),
    processing_time(gDtmp.processing_time),
    total_processing_time(gDtmp.total_processing_time),
    indice(gDtmp.indice), POP(gDtmp.POP),
    POP2(gDtmp.POP2), best_seq(gDtmp.best_seq),
    bestmakespan(gDtmp.bestmakespan),
    P_max(gDtmp.P_max),lambda(gDtmp.lambda), x(gDtmp.x),
    pls(gDtmp.pls),
    S_MIN(gDtmp.S_MIN),S_MAX(gDtmp.S_MAX),
    SIGMA_MIN(gDtmp.SIGMA_MIN),SIGMA_MAX(gDtmp.SIGMA_MAX),
    rho(gDtmp.rho)
{
    _dataMemoryPtr = new _dataMemory(n, m, processing_time, total_processing_time,
                                  indice, POP, POP2, best_seq, bestmakespan,
                                  lambda, x, P_max, S_MIN, S_MAX, SIGMA_MIN, SIGMA_MAX, pls, rho);
}

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
    P_max = this->_dataMemoryPtr->P_max;
    lambda = this->_dataMemoryPtr->lambda;
    x = this->_dataMemoryPtr->x;
    pls = this->_dataMemoryPtr->pls;
    S_MIN = this->_dataMemoryPtr->S_MIN;
    S_MAX = this->_dataMemoryPtr->S_MAX;
    SIGMA_MIN = this->_dataMemoryPtr->SIGMA_MIN;
    SIGMA_MAX = this->_dataMemoryPtr->SIGMA_MAX;
    rho = this->_dataMemoryPtr->rho;
}

// 定义全局数据对象
GlobalData globalData;
