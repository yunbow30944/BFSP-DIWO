//
// Created by DELL on 24-11-20.
//

#ifndef ALG2_H
#define ALG2_H
#include <random>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <vector>
using namespace std;

vector<int> reproduction(int S_min, int S_max);

vector<vector<int> > spatialDispersal(int k, vector<int> s, double sigma_min, double sigma_max);
#endif //ALG2_H
