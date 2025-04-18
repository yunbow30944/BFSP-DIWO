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
#define NO_IO_ALL
int ALG2::t_max;

// int main0() {
//     // IO::getBestSeqAndSave();
//     // IO::getARPDAndSave(10);
//
//     Test::makespanTest(1);
//     return 0;
// }

int main(){
    IO::getARPD_BestSeqAndSave(3);
    return 0;
}

int main0() {
    // 设置全局数据
    globalData.setData();
    
    // 设置CPU最大运行时间
    ALG2::t_max = ALG2::rho * globalData.m * globalData.n;

    ALG2::Sigma sig;
#ifndef NO_IO_ALL
    cout<<"================= ALG1 start =================\n"<<endl;
#endif
    ALG1(10, 10);
#ifndef NO_IO_ALL
    cout<<"================= ALG1 end =================\n"<<endl;

    cout << "================= POP AFTER ALG1: ======================" << endl;
    Utils::print_pop(globalData.POP);
    cout << "=======================================================" << endl;
#endif

    while(sig.getRunTime() <= ALG2::t_max){
#ifndef NO_IO_ALL
        cout<<"================= ALG2 start =================\n"<<endl;
#endif
        vector<int> s = reproduction(0, 7);
        globalData.POP2.clear();
        for (int i = 1; i <= globalData.P_max; i++) {
			spatialDispersal(s, 5, 10, sig);
        }
#ifndef NO_IO_ALL
        cout<<"================= ALG2 end =================\n"<<endl;

        cout << "=================POP2 AFTER ALG2:====================" << endl;
        Utils::print_pop(globalData.POP2);
        cout << "======================================================" << endl;

        cout << "\nBest_after alg2:" << endl;
        Utils::print_pi(globalData.best_seq);

        cout<<"================= ALG3 start =================\n"<<endl;
#endif
        for (int i = 0; i < globalData.POP2.size(); ++i) {
            std::default_random_engine e(time(0));
            std::uniform_real_distribution<double> _rand(0, 1);
            double rand = _rand(e);

            if (rand < globalData.pls) {
                globalData.POP2[i] = SRLS(globalData.POP2[i], globalData.best_seq);
            }

        }
#ifndef NO_IO_ALL
        cout<<"================= ALG3 end =================\n"<<endl;

        cout<<"================= ALG4 start =================\n"<<endl;
#endif
        competitiveExclusion(globalData.POP, globalData.POP2, globalData.P_max);
#ifndef NO_IO_ALL
        cout<<"================= ALG4 end =================\n"<<endl;

        cout << "=================POP AFTER ALG4:====================" << endl;
        Utils::print_pop(globalData.POP);
        cout << "======================================================" << endl;
#endif
    }

#ifdef IO_SHOW_FINAL_RESULT
    cout << "Best_seq_all:"<<endl;
    Utils::print_pi(globalData.best_seq);
#endif

    return 0;
}
