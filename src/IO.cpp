//
// Created by 周煜杰 on 25-1-17.
//
#define IO_SHOW_PROCESSING_DATA
#include "IO.h"

static const std::string datasetDirPath = "../data/NEH/tests";
// tests文件夹目录，默认程序在cmake-build-debug目录下，如果移动程序需要修改

namespace IO {

    /*
     *  @file: 文件读取流
     *  用于读取某文件中的某单个数据集
     *  与globalData.setData()大致相同
     *  返回值为得到的number of jobs, number of machines
     */
    pair<int, int> setData(std::ifstream &file) {
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
        ALG2::t_max = ALG2::rho * globalData.m * globalData.n;
        ALG2::Sigma sig;
        ALG1(10, 10);
        // for (int k = 1; k <= globalData.k_max; k++) {//TODO:substitute k
        while (sig.getRunTime() <= ALG2::t_max) {
            vector<int> s = reproduction(0, 7);
            globalData.POP2.clear();
            for (int i = 1; i <= globalData.P_max; i++) {
                spatialDispersal(s, 5, 10, sig);
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
#ifdef IO_SHOW_FINAL_RESULT
        cout << "Best_seq_all:";
        for (const auto &v: globalData.best_seq) {
            cout << v << " ";
        }
        cout << endl;
#endif
    }

    /*
     * @filePath: 文件路径
     * 读取指定文件路径下的文件并运行算法，
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

    /*
     *  用于写Cmin序列文件
     */
    void writeC_min(){
        std::ofstream fileOut("cmin_standard.txt", std::ios::out);
        fileOut << "Instance Best Source DIWO Instance Best Source DIWO Instance Best Source DIWO\n"
                   "20*5 50*10 100*20\n"
                   "Ta1 1374 1–15 1374 Ta41 3611 10 3614 Ta81 7709 15 7709\n"
                   "Ta2 1408 1–15 1408 Ta42 3470 14 3473 Ta82 7744 14 7769\n"
                   "Ta3 1280 1–15 1280 Ta43 3465 15 3465 Ta83 7723 14 7725\n"
                   "Ta4 1448 1–15 1448 Ta44 3650 8 3654 Ta84 7743 14 7755\n"
                   "Ta5 1341 1–15 1341 Ta45 3582 8 3619 Ta85 7730 13 7744\n"
                   "Ta6 1363 1–15 1363 Ta46 3571 14 3575 Ta86 7779 14 7785\n"
                   "Ta7 1381 1–15 1381 Ta47 3667 14 3669 Ta87 7857 15 7857\n"
                   "Ta8 1379 1–15 1379 Ta48 3549 15 3549 Ta88 7898 11 7902\n"
                   "Ta9 1373 1–15 1373 Ta49 3508 8 3518 Ta89 7818 14 7835\n"
                   "Ta10 1283 1–15 1283 Ta50 3608 12 3610 Ta90 7842 15 7842\n"
                   "20*10 50*20 200*10\n"
                   "Ta11 1698 1–15 1698 Ta51 4479 15 4479 Ta91 13149 14 13166\n"
                   "Ta12 1833 1–15 1833 Ta52 4262 14,15 4262 Ta92 13085 15 13085\n"
                   "Ta13 1659 1–15 1659 Ta53 4261 12,15 4261 Ta93 13183 15 13183\n"
                   "Ta14 1535 1–15 1535 Ta54 4339 12 4342 Ta94 13097 15 13097\n"
                   "Ta15 1617 1–15 1617 Ta55 4249 12 4250 Ta95 13100 15 13100\n"
                   "Ta16 1590 1–15 1590 Ta56 4271 12,15 4271 Ta96 12883 15 12883\n"
                   "Ta17 1622 1–15 1622 Ta57 4291 13,14 4296 Ta97 13408 15 13408\n"
                   "Ta18 1731 1–15 1731 Ta58 4298 14,15 4298 Ta98 13266 15 13266\n"
                   "Ta19 1747 1–15 1747 Ta59 4304 12 4305 Ta99 13080 15 13080\n"
                   "Ta20 1782 1–15 1782 Ta60 4398 15 4398 Ta100 13193 15 13193\n"
                   "20*20 100*5 200*20\n"
                   "Ta21 2436 1–15 2436 Ta61 6065 15 6065 Ta101 14192 9 14516\n"
                   "Ta22 2234 1–15 2234 Ta62 5934 15 5934 Ta102 14714 15 14714\n"
                   "Ta23 2479 1–15 2479 Ta63 5851 8 5876 Ta103 14831 15 14831\n"
                   "Ta24 2348 1–15 2348 Ta64 5656 9 5684 Ta104 14801 15 14801\n"
                   "Ta25 2435 1–15 2435 Ta65 5896 15 5896 Ta105 14609 15 14609\n"
                   "Ta26 2383 1–15 2383 Ta66 5755 15 5755 Ta106 14737 15 14737\n"
                   "Ta27 2390 1–15 2390 Ta67 5915 15 5915 Ta107 14770 15 14770\n"
                   "Ta28 2328 1–15 2328 Ta68 5809 14 5825 Ta108 14823 15 14823\n"
                   "Ta29 2363 1–15 2363 Ta69 6027 15 6027 Ta109 14691 15 14691\n"
                   "Ta30 2323 1–15 2323 Ta70 6059 14,15 6059 Ta110 14711 15 14711\n"
                   "50*5 100*10 500*20\n"
                   "Ta31 2980 11 2983 Ta71 6906 15 6906 Ta111 35380 15 35380\n"
                   "Ta32 3180 15 3180 Ta72 6656 15 6656 Ta112 35736 15 35736\n"
                   "Ta33 2995 14 3000 Ta73 6797 14 6807 Ta113 35406 15 35406\n"
                   "Ta34 3115 15 3115 Ta74 7035 15 7035 Ta114 35030 13 35738\n"
                   "Ta35 3139 8 3145 Ta75 6728 15 6728 Ta115 35417 15 35417\n"
                   "Ta36 3158 13 3161 Ta76 6537 14 6549 Ta116 35740 15 35740\n"
                   "Ta37 3005 11 3008 Ta77 6689 15 6689 Ta117 35299 15 35299\n"
                   "Ta38 3042 11 3044 Ta78 6746 14,15 6746 Ta118 35515 15 35515\n"
                   "Ta39 2889 13,14 2891 Ta79 6928 10 6958 Ta119 35268 15 35268\n"
                   "Ta40 3097 14 3102 Ta80 6855 11 6881 Ta120 35609 15 35609"
                   << std::endl;
        fileOut.close();
    }

    /*
     *  @R: 运行次数
     *  @C_min: 此算例下的最优解的完工时间
     *  @C_i: 此算例下某算法运行R次得到的序列
     *  计算单个的算例的ARPD
     *  本ARPD的计算将原公式化简推导后得到
     */
    double ARPD(const int &R, const int &C_min, const vector<int> &C_i){
        if(R <= 0)
            throw std::runtime_error("ARPD error: R <= 0!");
        else if(C_min <= 0)
            throw std::runtime_error("ARPD error: C_min <= 0!");
        else if(R != C_i.size())
            throw std::runtime_error("ARPD error: R!= C_i.size()!");
        int C_sum = std::accumulate(C_i.begin(), C_i.end(), 0);
        double ans = (static_cast<double>(100*C_sum)/static_cast<double>(R*C_min)) - 100.0;
        return ans;
    }

    /*
     *  @R: 运行次数
     *  @C_min: 此算例下的最优完工时间
     *  计算单个算例的ARPD
     */
    double run_single_ARPD_Calculate(const int &R, const int &C_min, const GlobalData &memory){
        vector<int> C_i;

        for(int i = 0; i < R; i ++){
            globalData.resetData();
            auto[jobNum, machineNum] = loadMemory(memory);   // 设置globalData
            runSingleExample(); // 使用算法跑当前数据集样例
            C_i.push_back(globalData.best_seq[0]);
        }

#ifdef IO_SHOW_PROCESSING_DATA
        cout << "=============================Ci array in ARPD calculation=============================\n";
        for(const auto &i: C_i)
            cout << i << " ";
        cout << "\n=====================================================================================\n";
#endif

        double result = ARPD(R, C_min, C_i);
#ifdef IO_SHOW_FINAL_RESULT
        cout << "ARPD: " << result << std::endl;
#endif
        return result;
    }

    /*
     *  @memory: 上一次运行算法前的存储的数据集相关数据
     *  将上一次运行算法前的数据加载到globalData中，便于重复运行同一个数据集
     */
    pair<int, int> loadMemory(const GlobalData &memory){
        globalData.n = memory.n;
        globalData.m = memory.m;
        globalData.processing_time.assign(memory.n+1, std::vector<int>(memory.m+1, 0));
        globalData.total_processing_time.assign(memory.n+1, 0);
        globalData.indice.assign(memory.n+1, 0);
        std::iota(globalData.indice.begin(), globalData.indice.end(), 0);
        for(int i = 1; i <= memory.m; i ++)
            for(int j = 1; j <= memory.n; j ++){
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
     *  处理计算单个文件的ARPD
     */
    void singleFileProcessWithARPD(const std::string &filePath, std::ofstream &fileOut, std::unordered_map<std::string, std::vector<int>> &C_min_map, const int &R){
        std::ifstream file(filePath);
        if(!file.is_open())
            throw std::domain_error(filePath+" not found!");
        string line;
        fileOut << "======================================================\n"
                << "File Name: " << filePath << "\n" << std::endl;
        int cnt = 1;
        while(std::getline(file, line)){
            globalData.resetData();
            auto[jobNum, machineNum] = setData(file);
            fileOut << "Num of jobs: " << jobNum << std::endl
                    << "Num of machines: " << machineNum << std::endl
                    << "Dataset: " << cnt << std::endl;
            GlobalData memory(globalData);
            std::string datasetName = to_string(jobNum) + "*" + to_string(machineNum);
            double ARPD_Result = run_single_ARPD_Calculate(R, C_min_map[datasetName][cnt], memory);
            fileOut << "ARPD: " << ARPD_Result << "\n" << std::endl;
            cnt++;
        }
        fileOut << "======================================================" << std::endl;
        file.close();
    }

    /*
     * @R: 计算ARPD时的运行次数
     * 用于计算ARPD并存储
     */
    void getARPDAndSave(const int &R){
        if(!std::filesystem::exists(datasetDirPath) || !std::filesystem::is_directory(datasetDirPath))
            throw std::domain_error("Invalid datasetDirPath!");

        std::ofstream fileOut("ARPD.txt", std::ios::app);

        writeC_min();
        // C_min_map用于存储C_min序列，std::string用于存储数据集的名称n*m，std::vector用于存储第几个数据集存储的C_min
        std::unordered_map<std::string, std::vector<int>> C_min_map;

        // 使用正则表达式提取出文件名中算例大小并可将算例大小加入map中
        std::regex pattern("t_j(\\d+)_m(\\d+)\\.txt");
        std::smatch match;

        // 填充C_min_map
        // 遍历目录
        for (const auto& entry : std::filesystem::directory_iterator(datasetDirPath)) {
            if (std::filesystem::is_regular_file(entry.status())) { // 判断是否是文件
                std::string fileName = entry.path().filename().string();
                if(std::regex_match(fileName, match, pattern)){
                    std::string datasetName = match[1].str() + "*" + match[2].str();
                    std::vector<int> C_min;
                    C_min.push_back(-1);    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    C_min_map.insert({datasetName, C_min});
                }
            }
        }
        // 从cmin_standard.txt中加载标准Cmin
        ifstream CminFile("cmin_standard.txt");
        if(!CminFile.is_open())
            throw std::runtime_error("cmin_standard.txt missing!");
        std::string line;
        std::getline(CminFile, line);
        while(std::getline(CminFile, line)){
            std::string datasetName1, datasetName2, datasetName3;
            istringstream iss_dataset(line);
            iss_dataset >> datasetName1 >> datasetName2 >> datasetName3;
            for(int i = 0; i < 10; i ++){
                std::getline(CminFile, line);
                istringstream iss(line);
                std::string cmin1, cmin2, cmin3, trash;
                iss
                    >> trash >> cmin1 >> trash >> trash
                    >> trash >> cmin2 >> trash >> trash
                    >> trash >> cmin3 >> trash >> trash;
                C_min_map[datasetName1].push_back(std::stoi(cmin1));
                C_min_map[datasetName2].push_back(std::stoi(cmin2));
                C_min_map[datasetName3].push_back(std::stoi(cmin3));
            }
        }

        // 计算每个文件中的所有的数据集的ARPD并与最佳序列一起保存
        // 遍历目录
        for (const auto& entry : std::filesystem::directory_iterator(datasetDirPath)) {
            if (std::filesystem::is_regular_file(entry.status())) { // 判断是否是文件
                string filePath = datasetDirPath +'/'+ entry.path().filename().string();
#ifdef IO_SHOW_PROCESSING_FILE
                std::cout << "Processing file: " << filePath << std::endl;
#endif
                singleFileProcessWithARPD(filePath, fileOut, C_min_map, R);
            }
        }

        fileOut.close();
    }
};