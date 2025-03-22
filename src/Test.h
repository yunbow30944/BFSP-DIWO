//
// Created by 周煜杰 on 25-2-2.
//

#ifndef TEST_H
#define TEST_H

#include <random>
#include "Test.h"
#include "GlobalData.h"
#include "Utils.h"
#include "Alg2.h"
#include "Alg3.h"
#include "Alg4.h"

#define IO_SHOW_INPUT_DATA  // 读取数据时确定是否需要在控制台输出读取的数据，若不输出则注释掉此句
#define IO_SHOW_PROCESSING_DATA  // 数据处理时确定是否需要在控制台输出每个ALG处理完的数据，若不输出则注释掉此句
#define IO_SHOW_PROCESSING_FILE // 运行算法时在控制台输出当前正在处理哪个文件，若不输出则注释词句
#define IO_SHOW_FINAL_RESULT    // 运行时在控制台输出最终结果

namespace Test {
    std::vector<std::vector<int>> generateExample();
    pair<int, int> setData(const std::vector<std::vector<int>> &example);
    bool makespanTest(int testCounts);
}

#endif //TEST_H
