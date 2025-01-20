//
// Created by 周煜杰 on 25-1-17.
//

#include "IO.h"

#define IO_SHOW_INPUT_DATA  // 读取数据时确定是否需要在控制台输出读取的数据，若不输出则注释掉此句
#define IO_SHOW_PROCESSING_DATA  // 数据处理时确定是否需要在控制台输出每个ALG处理完的数据，若不输出则注释掉此句
#define IO_SHOW_PROCESSING_FILE // 运行算法时在控制台输出当前正在处理哪个文件，若不输出则注释词句

static const std::string datasetDirPath = "../../NEH/tests";
// tests文件夹目录，默认程序在cmake-build-debug目录下，如果移动程序需要修改

namespace IO {

    /*
     *  @file: 文件读取流
     *  用于读取某文件中的某单个数据集
     *  与globalData.setData()大致相同
     *  返回值为得到的number of jobs, number of machines
     */
    pair<int, int> setData(std::ifstream& file) {
        string line;
        getline(file, line);
        int n, m, initial_seed, upper_bound, lower_bound;
        std::istringstream iss(line);
        iss >> n >> m >> initial_seed >> upper_bound >> lower_bound;    // 读取n,m
        globalData.n = n;
        globalData.m = m;

#ifdef IO_SHOW_INPUT_DATA
        // 输出读取的数据
        std::cout << "The number of jobs: " << n << std::endl;
        std::cout << "The number of machines: " << m << std::endl;
        std::cout << "Initial seed: " << initial_seed << std::endl;
        std::cout << "Upper bound: " << upper_bound << std::endl;
        std::cout << "Lower bound: " << lower_bound << std::endl;
#endif

        globalData.processing_time.assign(n+1, std::vector<int>(m+1, 0));
        globalData.total_processing_time.assign(n+1, 0);
        globalData.indice.assign(n+1, 0);
        std::iota(globalData.indice.begin(), globalData.indice.end(), 0);

#ifdef IO_SHOW_INPUT_DATA
        std::cout << "\nThe processing time of each component on different machines:\n";
#endif

        std::getline(file, line);   // 去掉字符串行
        for (int i = 1; i <= m; i ++) {
            std::getline(file, line);

#ifdef IO_SHOW_INPUT_DATA
            std::cout << line;
#endif
            istringstream iss_processing_time(line);
            for (int j = 1; j <= n; j ++) {
                iss_processing_time >> globalData.processing_time[j][i];
                globalData.total_processing_time[j] += globalData.processing_time[j][i];
            }
        }
        Utils::sort_by_tot_processing_time(globalData.indice, globalData.total_processing_time);
        return {n, m};
    }

    /*
     * 使用算法运行单个实例，与main函数主体大致相同
     */
    void runSingleExample() {
        ALG1(10, 10);
        for (int k = 1; k <= globalData.k_max; k++) {//TODO:substitute k
            vector<int> s = reproduction(0, 7);
            globalData.POP2.clear();
            for (int i = 1; i <= globalData.P_max; i++) {
                spatialDispersal(k, s, 0, 5);
            }

#ifdef IO_SHOW_PROCESSING_DATA
            cout << "Best_now:" << endl;
            for (const auto &v: globalData.best_seq) {
                cout << v << " ";
            }
            cout << endl;
#endif

            for (int i = 0; i < globalData.POP2.size(); ++i) {
                std::default_random_engine e(time(0));
                std::uniform_real_distribution<double> _rand(0, 1);
                double rand = _rand(e);

                if (rand < globalData.pls) {
                    globalData.POP2[i] = SRLS(globalData.POP2[i], globalData.best_seq);
                }

#ifdef IO_SHOW_PROCESSING_DATA
                cout << "Best_now:" << endl;
                for (const auto &v: globalData.best_seq) {
                    cout << v << " ";
                }
                cout << endl;
#endif

            }
            competitiveExclusion(globalData.POP, globalData.POP2, globalData.P_max);

#ifdef IO_SHOW_PROCESSING_DATA
            cout << "=================POP AFTER ALG4:====================" << endl;
            for (const auto &v: globalData.POP) {
                for (auto e: v) {
                    cout << e << " ";
                }
                cout << endl;
            }
            cout << "======================================================" << endl;
#endif
        }
#ifdef IO_SHOW_PROCESSING_DATA
        cout << "Best_seq_all:";
        for (const auto &v: globalData.best_seq) {
            cout << v << " ";
        }
        cout << endl;
#endif
    }

    /*
     * @filePath: 文件路径
     * 读取制定文件路径下的文件并运行算法，
     * 将得到的序列存入文件
     */
    void singleFileProcess(const std::string& filePath, std::ofstream& fileOut) {
        std::ifstream file(filePath);
        if (!file.is_open())
            throw domain_error(filePath + " not found!");
        string line;
        fileOut << "======================================================" << std::endl
                << "File Name: " << filePath << "\n" << std::endl;
        int cnt = 1;
        while (std::getline(file, line)) {
            globalData.resetData();
            auto[jobNum, machineNum] = setData(file);   // 设置globalData
            runSingleExample(); // 使用算法跑当前数据集样例
            // 保存数据
            fileOut << "Num of jobs: " << jobNum << std::endl
                    << "Num of machines: " << machineNum << std::endl
                    << "Dataset: " << cnt << std::endl;
            cnt ++;
            fileOut << "Best_seq_all: " << std::endl;
            for (const auto &i: globalData.best_seq)
                fileOut << i << " ";
            fileOut << "\n" << std::endl;
        }
        fileOut << "======================================================" << std::endl;
        file.close();
    }

    /*
     * 创建bestSequence.txt文件并存储tests目录下所有的文件的最优序列
     */
    void getBestSeqAndSave() {
        if (!std::filesystem::exists(datasetDirPath) || !std::filesystem::is_directory(datasetDirPath)) {
            throw std::domain_error("Invalid datasetDirPath!");
        }

        std::ofstream fileOut("bestSequence.txt", std::ios::app);

        // 遍历目录
        for (const auto& entry : std::filesystem::directory_iterator(datasetDirPath)) {
            if (std::filesystem::is_regular_file(entry.status())) { // 判断是否是文件
                string filePath = datasetDirPath +'/'+ entry.path().filename().string();
#ifdef IO_SHOW_PROCESSING_FILE
                std::cout << "Processing file: " << filePath << std::endl;
#endif
                singleFileProcess(filePath, fileOut);
            }
        }
        fileOut.close();
    }
};