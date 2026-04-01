# BFSP-DIWO

## 项目概述

本项目用于解决一类变种 Flowshop 排程问题——阻塞流水线车间调度问题（BFSP, Blocking Flow-Shop Scheduling Problem）。

- 主要算法：离散杂草入侵优化（DIWO，Discrete Invasive Weed Optimization）
- 支持：C++  实现和大规模测试

## 仓库结构

- `src/`：C++ 源码
- `data/NEH/`：NEH 算法及数据集
- `result/`：结果输出与处理
  - `original/`：原始结果、处理脚本
  - `final/`：最终汇总数据
- `LICENSE`：许可信息
- `CMakeLists.txt`：C++ 构建

## 使用方式

1. 构建方式：

```bash
mkdir build && cd build
cmake ..          
make              
```

2. 运行时输入：

- `n`：工件数
- `m`：机器数
- `processingtime`：处理时间矩阵

## 参考文献

- Shao Z, Pi D, Shao W, et al. *An efficient discrete invasive weed optimization for blocking flow-shop scheduling problem*. Engineering Applications of Artificial Intelligence, 2019, 78: 124–141.
- Wang L, Pan Q K, Suganthan P N, et al. *A novel hybrid discrete differential evolution algorithm for blocking flow shop scheduling problems*. Computers & Operations Research, 2010, 37(3): 509–520.
- Pan Q K, Wang L. *Effective heuristics for the blocking flowshop scheduling problem with makespan minimization*. Omega, 2012, 40(2): 218–229. 

## LICENSE

- 本项目主要采用 MIT 许可（详见根目录 `LICENSE`）

## 历史记录

+ 2024.7.31 上传了第一版本的 NEH 算法
+ 8.9 更新了 RSB-PF_NEH，上传了 Alg1-NEH 算法
+ 9.19 上传 NEH 供参考
+ 10.4 ALG1 修改完成
+ 10.27 上传 ALG2
+ 11.20 重新构建项目结构
+ 11.21 ALG2 修改调试完成
+ 11.22 基础版 ALG3 构建完成，计算过程仍可优化速度
+ 11.22 全部构建完成 bug 待调
+ 11.23 bug 修改完成
+ 2025.1.16 修改细节
+ 3.23 加速算法修改完成
+ 4.26 完成调参
+ 4.27 结题！
+ 2026.4.2 开源代码仓库

## 后续方向

1. 尝试改进论文算法
1. 尝试优化算法时间复杂度
