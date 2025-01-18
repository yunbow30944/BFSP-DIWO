//
// Created by 周煜杰 on 25-1-17.
//

#ifndef IO_H
#define IO_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <filesystem>
#include "GlobalData.h"
#include "Utils.h"
#include "Alg1.h"
#include "Alg2.h"
#include "Alg3.h"
#include "Alg4.h"

namespace IO {
    pair<int, int> setData(std::ifstream& file);
    void singleFileProcess(const std::string& filePath, std::ofstream& fileOut);
    void runSingleExample();
    void getBestSeqAndSave();
};



#endif //IO_H
