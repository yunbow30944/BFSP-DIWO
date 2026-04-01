// SPDX-License-Identifier: MIT
// Copyright (c) 2026 yunbow30944, EpsilonZYJ, find-xin

#ifndef IO_H
#define IO_H

#include "Alg1.h"
#include "Alg2.h"
#include "Alg3.h"
#include "Alg4.h"
#include "GlobalData.h"
#include "Utils.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

// #define IO_SHOW_INPUT_DATA  //
// 读取数据时确定是否需要在控制台输出读取的数据，若不输出则注释掉此句 #define
// IO_SHOW_PROCESSING_DATA  //
// 数据处理时确定是否需要在控制台输出每个ALG处理完的数据，若不输出则注释掉此句
// #define IO_SHOW_PROCESSING_FILE //
// 运行算法时在控制台输出当前正在处理哪个文件，若不输出则注释词句 #define
// IO_SHOW_FINAL_RESULT    // 运行时在控制台输出最终结果

namespace IO {
void setData(std::ifstream &file);
void singleFileProcess(const std::string &filePath, std::ofstream &fileOut);
void runSingleExample();
void getBestSeqAndSave();
double run_single_ARPD_Calculate(const int &R, const int &C_min);
void loadMemory(const GlobalData &memory);
void singleFileProcessWithARPD(
    const std::string &filePath, std::ofstream &file,
    const std::unordered_map<std::string, std::vector<int>> &C_min_map,
    const int &R);
void getARPD_BestSeqAndSave(const int &R);
}; // namespace IO

#endif // IO_H
