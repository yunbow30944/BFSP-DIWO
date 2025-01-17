//
// Created by DELL on 24-11-22.
//

#include"Utils.h"
#include"Alg3.h"
#include"GlobalData.h"
using namespace std;

vector<int> SRLS(vector<int> pi, vector<int> pi_r) {
    //TODO: optimize
    int n = globalData.n;
    //cout<<"n = "<<n<<endl;
    int cnt = 1, j = 0, flag = 0;

    do {
        while (cnt < n) {
            j = (j % n) + 1; // 计算 pi_r 的位置，模运算循环
            vector<int> pi_prime = pi;

            // 从 pi 中移除 pi_r[j]
            auto it = find(pi_prime.begin() + 1, pi_prime.end(), pi_r[j]);
            if (it != pi_prime.end()) {
                pi_prime.erase(it);
            }

            int bestfit = INT_MAX;
            int keypos = -1;

            //尝试所有位置
            for (int i = 1; i <= n; ++i) {
                vector<int> pi_double_prime = pi_prime;
                pi_double_prime.insert(pi_double_prime.begin() + i, pi_r[j]);

                int cost = Utils::calculate(pi_double_prime, globalData.processing_time);
                if (cost < bestfit) {
                    bestfit = cost;
                    keypos = i;
                }
            }

            //更新
            if (pi[0] > bestfit) {
                pi_prime.insert(pi_prime.begin() + keypos, pi_r[j]);
                pi = pi_prime;
                pi[0] = bestfit;
                //更新当前最佳
                if (pi[0] < globalData.bestmakespan) {
                    globalData.bestmakespan = pi[0];
                    globalData.best_seq = pi;
                }
                cnt = 1;
            } else {
                ++cnt;
            }
        }

        if (flag == 0) {
            //随机打乱
            vector<int> shuffled_pi_r = pi_r;
            Utils::shuffleArray(shuffled_pi_r); //Fisher-yates

            pi_r = shuffled_pi_r;
            cnt = 1;
        }
        ++flag;
    } while (flag != 2);

    return pi;
}
