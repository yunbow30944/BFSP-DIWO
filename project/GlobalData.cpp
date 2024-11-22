//
// Created by DELL on 24-11-20.
//
#include "GlobalData.h"
using namespace std;

void GlobalData::setData() {
    std::cout << "The number of components: ";
    std::cin >> n;
    std::cout << "The number of machines: ";
    std::cin >> m;
    processing_time.assign(n + 1, std::vector<int>(m + 1, 0));
    total_processing_time.assign(n + 1, 0);
    indice.assign(n + 1, 0);
    iota(indice.begin(), indice.end(), 0); // 初始化组件索引0~n-1
    printf("\nThe processing time of each component on different machines:\n");
    for (int i = 1; i <= m; ++i)
        for (int j = 1; j <= n; ++j) {
            cin >> processing_time[j][i]; // 输入处理时间
            total_processing_time[j] += processing_time[j][i]; // 更新总处理时间
        }
    sort(indice.begin() + 1, indice.end(), [this](int a, int b) {
        return total_processing_time[a] < total_processing_time[b];
    });
}

// 定义全局数据对象
GlobalData globalData;
