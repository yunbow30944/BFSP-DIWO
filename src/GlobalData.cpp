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

// 清空运行时信息
void GlobalData::resetData() {
    POP.clear();
    POP2.clear();
    best_seq.clear();
    bestmakespan = 0;
}

// 定义全局数据对象
GlobalData globalData;
