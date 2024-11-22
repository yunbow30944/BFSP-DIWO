//
// Created by DELL on 24-11-20.
//
#include "GlobalData.h"
#include "Alg1.h"
#include "Alg2.h"
#include "Alg3.h"
#include "Utils.h"


int main() {
    // 设置全局数据
    globalData.setData();
    // 调用 ALG1 和 ALG2
    ALG1(500, 10);

    for (int k = 1; k <= globalData.k_max; k++) {
        vector<int> s = reproduction(0, 7);

        for (int i = 1; i <= globalData.P_max; i++) {
            spatialDispersal(k, s, 0, 5);
        }
        cout << "=================POP2 AFTER ALG2:====================" << endl;
        for (const auto &v: globalData.POP2) {
            for (auto e: v) {
                cout << e << " ";
            }
            cout << endl;
        }
        cout << "======================================================" << endl;

        cout<<"Best_now:"<<endl;
        for (const auto &v: globalData.best_seq) {
            cout<<v<<" ";
        }
        cout<<endl;


        for (int i = 0; i < globalData.POP2.size(); ++i) {
            std::default_random_engine e(time(0));
            std::uniform_real_distribution<double> _rand(0, 1);
            double rand = _rand(e);

            if(rand<globalData.pls) {
                globalData.POP2[i]=SRLS(globalData.POP2[i],globalData.best_seq);
            }

            cout<<"Best_now:"<<endl;
            for (const auto &v: globalData.best_seq) {
                cout<<v<<" ";
            }
            cout<<endl;
        }
    }

    return 0;
}
