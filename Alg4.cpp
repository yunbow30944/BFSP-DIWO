//
// Created by DELL on 24-11-22.
//
#include"Alg4.h"
#include"Utils.h"
#include"GlobalData.h"
using namespace std;


vector<vector<int> > competitiveExclusion(vector<vector<int> > &POP, vector<vector<int> > &POP_prime, const int P_max) {
    vector<vector<int> > POP_double_prime;

    // POP_double_prime = POP ∪ POP_prime
    POP_double_prime = POP;
    POP_double_prime.insert(POP_double_prime.end(), POP_prime.begin(), POP_prime.end());

    // 按成本（pi[0]）升序排序 POP_double_prime
    Utils::sortAllpi(POP_double_prime);

    POP.clear();

    int j = 1;
    POP.push_back(POP_double_prime[0]);

    // 竞争排除
    while (POP.size() < P_max && j < POP_double_prime.size()) {
        bool flag = false;
        if (Utils::is_unique(POP_double_prime[j], POP)) {
            flag = true;
        }
        // 如果没有，加入POP
        if (flag) {
            POP.push_back(POP_double_prime[j]);
        }

        ++j; // 递增 j
    }
    return POP;
}
