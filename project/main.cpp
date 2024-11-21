//
// Created by DELL on 24-11-20.
//
#include "GlobalData.h"
#include "Alg1.h"
#include "Alg2.h"
#include "Utils.h"

int main() {
    // 设置全局数据
    globalData.setData();
    // 调用 ALG1 和 ALG2
    ALG1(1000, 10);

    for (int k = 1; k <= globalData.k_max; k++) {
        vector<int> s = reproduction(0, 7);

        for (int i = 1; i <= globalData.P_max; i++) {
            spatialDispersal(k, s, 0, 5);
        }
        cout<<"=================POP2 AFTER ALG2:===================="<<endl;
        for(const auto& v : globalData.POP2) {
            for(auto e:v) {
                cout << e << " ";
            }
            cout << endl;
        }
        cout<<"==============================================="<<endl;

    }

    return 0;
}
