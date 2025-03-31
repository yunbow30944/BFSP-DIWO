//
// Created by DELL on 24-11-20.
//
#include "GlobalData.h"
#include "Alg1.h"
#include "Alg2.h"
#include "Alg3.h"
#include "Alg4.h"
#include "IO.h"
#include "Test.h"
#include "Utils.h"

int ALG2::t_max;


// int main0() {
//     // IO::getBestSeqAndSave();
//     // IO::getARPDAndSave(10);
//
//     Test::makespanTest(1);
//     return 0;
// }

int main() {
    // 设置全局数据
    globalData.setData();
    
    // 设置CPU最大运行时间
    ALG2::t_max = ALG2::rho * globalData.m * globalData.n;

    ALG2::Sigma sig;
    cout<<"================= ALG1 start =================\n"<<endl;
    // 调用 ALG1
    ALG1(10, 10);
    cout<<"================= ALG1 end =================\n"<<endl;


    // for (int k = 1; k <= 10; k++) {//TODO:substitute k
    while(sig.getRunTime() <= ALG2::t_max){
        cout<<"================= ALG2 start =================\n"<<endl;
        vector<int> s = reproduction(0, 7);
        globalData.POP2.clear();
        for (int i = 1; i <= globalData.P_max; i++) {
            // spatialDispersal(k, s, 0, 5);
			spatialDispersal(s, 0, 5, sig);
        }
        cout<<"================= ALG2 end =================\n"<<endl;

        cout << "=================POP2 AFTER ALG2:====================" << endl;
        Utils::print_pop(globalData.POP2);
        cout << "======================================================" << endl;

        cout << "\nBest_after alg2:" << endl;
        Utils::print_pi(globalData.best_seq);


        cout<<"================= ALG3 start =================\n"<<endl;
        for (int i = 0; i < globalData.POP2.size(); ++i) {
            std::default_random_engine e(time(0));
            std::uniform_real_distribution<double> _rand(0, 1);
            double rand = _rand(e);

            if (rand < globalData.pls) {
                globalData.POP2[i] = SRLS(globalData.POP2[i], globalData.best_seq);
            }

            // cout << "Best_now:" << endl;
            // for (const auto &v: globalData.best_seq) {
            //     cout << v << " ";
            // }
            // cout << endl;
        }
        cout<<"================= ALG3 end =================\n"<<endl;

        cout<<"================= ALG4 start =================\n"<<endl;
        competitiveExclusion(globalData.POP, globalData.POP2, globalData.P_max);
        cout<<"================= ALG4 end =================\n"<<endl;

        cout << "=================POP AFTER ALG4:====================" << endl;
        Utils::print_pop(globalData.POP);
        cout << "======================================================" << endl;
    }

    cout << "Best_seq_all:";
    for (const auto &v: globalData.best_seq) {
        cout << v << " ";
    }
    cout << endl;

    return 0;
}
