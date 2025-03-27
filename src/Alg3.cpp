//
// Created by DELL on 24-11-22.
//

#include"Utils.h"
#include"Alg3.h"
#include"GlobalData.h"
using namespace std;

vector<int> SRLS(vector<int> pi, vector<int> pi_r) {

    int n = globalData.n;
    int m = globalData.m;
    vector<vector<int> > processing_time = globalData.processing_time;

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
            vector<vector<int> > e_2(n + 1, vector<int>(m + 1, 0));
            Utils::calculate_departure_time(e_2, 1, n - 1, pi_prime, processing_time); //得到初始的e''

            vector<vector<int> > f_2(n + 1, vector(m + 2, 0));
            Utils::calculate_tail_time(f_2, n - 1, 1, pi_prime, processing_time); //得到初始的f''

            int bestfit = INT_MAX;
            int keypos = -1;
            pair<int,int> ans = Utils::neighbor_insertion(n-1,pi_r[j],e_2,f_2,processing_time);
            keypos = ans.first;bestfit = ans.second;

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
