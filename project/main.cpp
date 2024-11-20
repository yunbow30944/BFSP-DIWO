//
// Created by DELL on 24-11-20.
//
#include "GlobalData.h"
#include "Alg1.h"
//#include "Alg2.h"
#include "Utils.h"

int main() {
    // 设置全局数据
    globalData.setData();

    // 调用 ALG1 和 ALG2
    ALG1(1000);
    //ALG2();

    return 0;
}
