//
// Created by DELL on 24-11-20.
//

#ifndef UTILS_H
#define UTILS_H


#include <vector>
#include <iostream>
#include<climits>
#include<random>
#include <vector>
#include<algorithm>
using namespace std;

class Utils {
public:
    static void remove_non_improving_moves(const vector<vector<int>> &e, const vector<vector<int>> &f, const int c_max, vector<vector<int>> &V, const vector<int> order, const vector<vector<int>> processing_time);

    static void calculate_tail_time(vector<vector<int>> &f, const int begin, const int end, const vector<int> &order, const vector<vector<int>> &processing_time);

    static void calculate_departure_time(vector<vector<int>> &e, const int begin, const int end, const vector<int> &order, const vector<vector<int>> &processing_time);

    static int calculate(const vector<int> &order, const vector<vector<int>> &processing_time);

    static vector<vector<int> > calculate_v(int &v, int i, const vector<int> &order, vector<vector<int> > d,
                                            const vector<vector<int> > &processing_time);

    static vector<int> random_init(int n);

    static bool is_unique(const vector<int> &seq, const vector<vector<int> > &POP);

    static void sort_by_tot_processing_time(vector<int> &pi_R, const vector<int> &total_processing_time);

    static vector<int> findBestpi(const vector<vector<int> > &pop);

    static void sortAllpi(vector<vector<int> > &pop);

    static void shuffleArray(vector<int> &array);

    static void print_pop(vector<vector<int>> pop);

    static void print_pi(vector<int> order);

    static double ARPD(const int &R, const int &C_min, const vector<int> &C_i);

    static pair<int, int> neighbor_insertion(int length, int job, vector<vector<int>> e_2, vector<vector<int>> f_2,
                                      vector<vector<int>> processing_time);
};

#endif //UTILS_H
