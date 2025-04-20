//
// Created by 周煜杰 on 25-1-17.
//
// #define IO_SHOW_PROCESSING_DATA
#include "IO.h"
#define IO_SHOW_FINAL_RESULT
#define NO_IO_ALL

static const std::string datasetDirPath = "../data/NEH/tests";
// tests文件夹目录，默认程序在cmake-build-debug目录下，如果移动程序需要修改

namespace IO
{
    /*
     *  @file: 文件读取流
     *  用于读取某文件中的某单个数据集
     *  与globalData.setData()大致相同
     *  返回值为得到的number of jobs, number of machines
     */
    pair<int, int> setData(std::ifstream &file)
    {
        string line;
        getline(file, line);
        int n, m, initial_seed, upper_bound, lower_bound;
        std::istringstream iss(line);
        iss >> n >> m >> initial_seed >> upper_bound >> lower_bound; // 读取n,m
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

        globalData.processing_time.assign(n + 1, std::vector<int>(m + 1, 0));
        globalData.total_processing_time.assign(n + 1, 0);
        globalData.indice.assign(n + 1, 0);
        std::iota(globalData.indice.begin(), globalData.indice.end(), 0);

#ifdef IO_SHOW_INPUT_DATA
        std::cout << "\nThe processing time of each component on different machines:\n";
#endif

        std::getline(file, line); // 去掉字符串行
        for (int i = 1; i <= m; i++)
        {
            std::getline(file, line);

#ifdef IO_SHOW_INPUT_DATA
            std::cout << line;
#endif
            istringstream iss_processing_time(line);
            for (int j = 1; j <= n; j++)
            {
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
    void runSingleExample()
    {
        // 设置CPU最大运行时间
        ALG2::t_max = globalData.rho * globalData.m * globalData.n;

        ALG2::Sigma sig;
#ifndef NO_IO_ALL
        cout << "================= ALG1 start =================\n"
             << endl;
#endif
        ALG1(globalData.P_max, globalData.lambda,globalData.x);
#ifndef NO_IO_ALL
        cout << "================= ALG1 end =================\n"
             << endl;

        cout << "================= POP AFTER ALG1: ======================" << endl;
        Utils::print_pop(globalData.POP);
        cout << "=======================================================" << endl;
#endif

        while (sig.getRunTime() <= ALG2::t_max)
        {
#ifndef NO_IO_ALL
            cout << "================= ALG2 start =================\n"
                 << endl;
#endif
            vector<int> s = reproduction(globalData.S_MIN, globalData.S_MAX);
            globalData.POP2.clear();
            for (int i = 1; i <= globalData.P_max; i++)
            {
                spatialDispersal(s, globalData.SIGMA_MIN, globalData.SIGMA_MAX, sig);
            }
#ifndef NO_IO_ALL
            cout << "================= ALG2 end =================\n"
                 << endl;

            cout << "=================POP2 AFTER ALG2:====================" << endl;
            Utils::print_pop(globalData.POP2);
            cout << "======================================================" << endl;

            cout << "\nBest_after alg2:" << endl;
            Utils::print_pi(globalData.best_seq);

            cout << "================= ALG3 start =================\n"
                 << endl;
#endif
            for (int i = 0; i < globalData.POP2.size(); ++i)
            {
                std::default_random_engine e(time(0));
                std::uniform_real_distribution<double> _rand(0, 1);
                double rand = _rand(e);

                if (rand < globalData.pls)
                {
                    globalData.POP2[i] = SRLS(globalData.POP2[i], globalData.best_seq);
                }
            }
#ifndef NO_IO_ALL
            cout << "================= ALG3 end =================\n"
                 << endl;

            cout << "================= ALG4 start =================\n"
                 << endl;
#endif
            competitiveExclusion(globalData.POP, globalData.POP2, globalData.P_max);
#ifndef NO_IO_ALL
            cout << "================= ALG4 end =================\n"
                 << endl;

            cout << "=================POP AFTER ALG4:====================" << endl;
            Utils::print_pop(globalData.POP);
            cout << "======================================================" << endl;
#endif
        }

#ifdef IO_SHOW_FINAL_RESULT
        cout << "Best_seq_all:" << endl;
        Utils::print_pi(globalData.best_seq);
#endif
    }

    /*
     * @filePath: 文件路径
     * 读取指定文件路径下的文件并运行算法，
     * 将得到的序列存入文件
     */
    void singleFileProcess(const std::string &filePath, std::ofstream &fileOut)
    {
        std::ifstream file(filePath);
        if (!file.is_open())
            throw domain_error(filePath + " not found!");
        string line;
        int cnt = 1;
        while (std::getline(file, line))
        {
            globalData.resetData();
            auto [jobNum, machineNum] = setData(file); // 设置globalData
            runSingleExample();                        // 使用算法跑当前数据集样例
            // 保存数据
            fileOut << filePath << "," << jobNum << "," << machineNum << "," << cnt << ",";

            cnt++;
            for (const auto &i : globalData.best_seq)
                fileOut << i << ",";
            fileOut << "\n";
        }
        file.close();
    }

    /*
     * 创建bestSequence.csv文件并存储tests目录下所有的文件的最优序列
     */
    void getBestSeqAndSave()
    {
        if (!std::filesystem::exists(datasetDirPath) || !std::filesystem::is_directory(datasetDirPath))
        {
            throw std::domain_error("Invalid datasetDirPath!");
        }

        std::ofstream fileOut("../data/bestSequence.csv", std::ios::app);
        fileOut << "File path,Num of jobs,Num of machines,Dataset,Best sequence of all" << std::endl;
        // 遍历目录
        for (const auto &entry : std::filesystem::directory_iterator(datasetDirPath))
        {
            if (std::filesystem::is_regular_file(entry.status()))
            {
                // 判断是否是文件
                string filePath = datasetDirPath + '/' + entry.path().filename().string();
#ifdef IO_SHOW_PROCESSING_FILE
                std::cout << "Processing file: " << filePath << std::endl;
#endif
                singleFileProcess(filePath, fileOut);
                fileOut.flush();
            }
        }
        fileOut.close();
    }


    /*
     *  @R: 运行次数
     *  @C_min: 此算例下的最优完工时间
     *  @memory: 数据集内容
     *  运行并计算单个算例的ARPD，返回ARPD
     */
    double run_single_ARPD_Calculate(const int &R, const int &C_min, const GlobalData &memory)
    {
        vector<int> C_i;

        for (int i = 0; i < R; i++)
        {
            globalData.resetData();
            auto [jobNum, machineNum] = loadMemory(memory); // 设置globalData
            runSingleExample();                             // 使用算法跑当前数据集样例
            C_i.push_back(globalData.best_seq[0]);
        }

#ifdef IO_SHOW_PROCESSING_DATA
        cout << "=============================Ci array in ARPD calculation=============================\n";
        for (const auto &i : C_i)
            cout << i << " ";
        cout << "\n=====================================================================================\n";
#endif

        double result = Utils::ARPD(R, C_min, C_i);
#ifdef IO_SHOW_FINAL_RESULT
        cout << "ARPD: " << result << std::endl;
#endif
        return result;
    }

    /*
     *  @memory: 上一次运行算法前的存储的数据集相关数据
     *  将上一次运行算法前的数据加载到globalData中，便于重复运行同一个数据集
     */
    pair<int, int> loadMemory(const GlobalData &memory)
    {
        globalData.n = memory.n;
        globalData.m = memory.m;
        globalData.processing_time.assign(memory.n + 1, std::vector<int>(memory.m + 1, 0));
        globalData.total_processing_time.assign(memory.n + 1, 0);
        globalData.indice.assign(memory.n + 1, 0);
        std::iota(globalData.indice.begin(), globalData.indice.end(), 0);
        for (int i = 1; i <= memory.m; i++)
            for (int j = 1; j <= memory.n; j++)
            {
                globalData.processing_time[j][i] = memory.processing_time[j][i];
                globalData.total_processing_time[j] += globalData.processing_time[j][i];
            }
        Utils::sort_by_tot_processing_time(globalData.indice, globalData.total_processing_time);
        return {memory.n, memory.m};
    }


    /*
     *  @filePath: 数据集文件路径
     *  @fileOut: 输出文件流
     *  @C_min_map: 存储C_min
     *  @R: 单个数据集的运行次数
     *  计算同一规模算例的ARPD，每个跑R次，写入csv文件
     */
    void singleFileProcessWithARPD(const std::string &filePath, std::ofstream &fileOut,
                                             std::unordered_map<std::string, std::vector<int>> &C_min_map,
                                             const int &R)
    {
        std::ifstream file(filePath);
        if (!file.is_open())
            throw std::domain_error(filePath + " not found!");
        string line;
        int cnt = 1;
        while (std::getline(file, line))
        {
            globalData.resetData();
            auto [jobNum, machineNum] = setData(file);
            GlobalData memory(globalData);
            std::string datasetName = to_string(jobNum) + "*" + to_string(machineNum);
            for (int i = 0; i < R; i++)
            {
                double ARPD_Result = run_single_ARPD_Calculate(1, C_min_map[datasetName][cnt], memory);
                fileOut << filePath << "," << jobNum << "," << machineNum << "," << cnt << "," << ARPD_Result << ",";
                for (const auto &i : globalData.best_seq)
                    fileOut << i << ",";
                fileOut << "\n";
            }
            cnt++;
        }
        file.close();
    }


    /*
     * @R: 计算ARPD时的运行次数
     * 用于计算ARPD并存储ARPD与BestSequence
     */
    void getARPD_BestSeqAndSave(const int &R)
    {
        if (!std::filesystem::exists(datasetDirPath) || !std::filesystem::is_directory(datasetDirPath))
            throw std::domain_error("Invalid datasetDirPath!");

       // std::ofstream fileOut("BestSeq_ARPD.csv", std::ios::app);
        std::ofstream fileOut("../data/BestSeq_ARPD.csv", std::ios::trunc);//若存在，先删除再写入
        if (!fileOut.is_open()) {
            std::cerr << "Failed to create/open file!" << std::endl;
            // 错误处理
        }

        //writeC_min();
        // C_min_map用于存储C_min序列，std::string用于存储数据集的名称n*m，std::vector用于存储第几个数据集存储的C_min
        std::unordered_map<std::string, std::vector<int>> C_min_map;

        // 使用正则表达式提取出文件名中算例大小并可将算例大小加入map中
        std::regex pattern(R"(t_j(\d+)_m(\d+)\.txt)");
        std::smatch match;

        // 填充C_min_map
        // 遍历目录
        for (const auto &entry : std::filesystem::directory_iterator(datasetDirPath))
        {
            if (std::filesystem::is_regular_file(entry.status()))
            {
                // 判断是否是文件
                std::string fileName = entry.path().filename().string();
                if (std::regex_match(fileName, match, pattern))
                {
                    std::string datasetName = match[1].str() + "*" + match[2].str();
                    std::vector<int> C_min;
                    C_min.push_back(-1);
                    C_min_map.insert({datasetName, C_min});
                }
            }
        }
        // 从cmin_standard.txt中加载标准Cmin
        ifstream CminFile("../data/cmin_standard.txt");
        if (!CminFile.is_open())
            throw std::runtime_error("cmin_standard.txt missing!");
        std::string line;
        std::getline(CminFile, line);
        while (std::getline(CminFile, line))
        {
            std::string datasetName1, datasetName2, datasetName3;
            istringstream iss_dataset(line);
            iss_dataset >> datasetName1 >> datasetName2 >> datasetName3;
            for (int i = 0; i < 10; i++)
            {
                std::getline(CminFile, line);
                istringstream iss(line);
                std::string cmin1, cmin2, cmin3, trash;
                iss >> trash >> cmin1 >> trash >> trash >> trash >> cmin2 >> trash >> trash >> trash >> cmin3 >> trash >> trash;
                C_min_map[datasetName1].push_back(std::stoi(cmin1));
                C_min_map[datasetName2].push_back(std::stoi(cmin2));
                C_min_map[datasetName3].push_back(std::stoi(cmin3));
            }
        }

        // 计算每个文件中的所有的数据集的ARPD并与最佳序列一起保存
        // 遍历目录
        fileOut << "File path,Num of jobs,Num of machines,Dataset,ARPD,Best sequence of all" << std::endl;
        for (const auto &entry : std::filesystem::directory_iterator(datasetDirPath))
        {
            if (std::filesystem::is_regular_file(entry.status()))
            {
                // 判断是否是文件
                string filePath = datasetDirPath + '/' + entry.path().filename().string();
#ifdef IO_SHOW_PROCESSING_FILE
                std::cout << "Processing file: " << filePath << std::endl;
#endif
                singleFileProcessWithARPD(filePath, fileOut, C_min_map, R);
                fileOut.flush();
            }
        }

        fileOut.close();
    }
};
