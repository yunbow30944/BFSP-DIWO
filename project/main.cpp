//
// Created by DELL on 24-11-20.
//
#include "GlobalData.h"
#include "Alg1.h"
#include "Alg2.h"
#include "Alg3.h"
#include "Alg4.h"
#include "IO.h"
#include "Utils.h"

int main0() {
    IO::getBestSeqAndSave();
    return 0;
}

int main() {
    // 设置全局数据
    globalData.setData();
    // 调用 ALG1
    ALG1(10, 10);

    for (int k = 1; k <= globalData.k_max; k++) {//TODO:substitute k
        vector<int> s = reproduction(0, 7);
        globalData.POP2.clear();
        for (int i = 1; i <= globalData.P_max; i++) {
            spatialDispersal(k, s, 0, 5);
        }
        // cout << "=================POP2 AFTER ALG2:====================" << endl;
        // for (const auto &v: globalData.POP2) {
        //     for (auto e: v) {
        //         cout << e << " ";
        //     }
        //     cout << endl;
        // }
        // cout << "======================================================" << endl;

        cout << "Best_now:" << endl;
        for (const auto &v: globalData.best_seq) {
            cout << v << " ";
        }
        cout << endl;


        for (int i = 0; i < globalData.POP2.size(); ++i) {
            std::default_random_engine e(time(0));
            std::uniform_real_distribution<double> _rand(0, 1);
            double rand = _rand(e);

            if (rand < globalData.pls) {
                globalData.POP2[i] = SRLS(globalData.POP2[i], globalData.best_seq);
            }

            cout << "Best_now:" << endl;
            for (const auto &v: globalData.best_seq) {
                cout << v << " ";
            }
            cout << endl;
        }
        competitiveExclusion(globalData.POP, globalData.POP2, globalData.P_max);
        cout << "=================POP AFTER ALG4:====================" << endl;
        for (const auto &v: globalData.POP) {
            for (auto e: v) {
                cout << e << " ";
            }
            cout << endl;
        }
        cout << "======================================================" << endl;
    }

    cout << "Best_seq_all:";
    for (const auto &v: globalData.best_seq) {
        cout << v << " ";
    }
    cout << endl;

    return 0;
}
