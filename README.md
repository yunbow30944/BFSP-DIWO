  # BFSP-DIWO

  ## Project Overview

  This project addresses a variant of the Flowshop scheduling problem—the Blocking Flow-Shop Scheduling Problem (BFSP).

  - Main algorithm: Discrete Invasive Weed Optimization (DIWO)
  - Implementation: C++ with large-scale testing support

  ## Repository Structure

  - `src/`: C++ source code
  - `data/NEH/`: NEH algorithm and datasets
  - `result/`: Result output and processing
    - `original/`: Raw results, processing scripts
    - `final/`: Final aggregated data
  - `LICENSE`: License information
  - `CMakeLists.txt`: C++ build configuration

  ## Usage

  1. Build:

  ```bash
  mkdir build && cd build
  cmake ..          
  make              
  ```

  2. Runtime inputs:

  - `n`: Number of jobs
  - `m`: Number of machines
  - `processingtime`: Processing time matrix

  ## References

  - Shao Z, Pi D, Shao W, et al. *An efficient discrete invasive weed optimization for blocking flow-shop scheduling problem*. Engineering Applications of Artificial Intelligence, 2019, 78: 124–141.
  - Wang L, Pan Q K, Suganthan P N, et al. *A novel hybrid discrete differential evolution algorithm for blocking flow shop scheduling problems*. Computers & Operations Research, 2010, 37(3): 509–520.
  - Pan Q K, Wang L. *Effective heuristics for the blocking flowshop scheduling problem with makespan minimization*. Omega, 2012, 40(2): 218–229.

  ## LICENSE

  - This project is primarily licensed under the MIT License (see `LICENSE` in the root directory).

  ## Change Log

  + 2024.07.31 Initial upload of NEH algorithm
  + 08.09 Updated RSB-PF_NEH, uploaded Alg1-NEH algorithm
  + 09.19 Uploaded NEH for reference
  + 10.04 Completed ALG1 modifications
  + 10.27 Uploaded ALG2
  + 11.20 Restructured project layout
  + 11.21 Completed ALG2 debugging
  + 11.22 Completed basic ALG3 implementation; computation speed can still be optimized
  + 11.22 Full implementation completed; bugs pending
  + 11.23 Bug fixes completed
  + 2025.01.16 Minor refinements
  + 03.23 Completed speed-up algorithm implementation
  + 04.26 Completed parameter tuning
  + 04.27 Project concluded!
  + 2026.4.2 Open source the codebase
  ## Future Directions

  1. Attempt to improve the paper's algorithm
  2. Attempt to optimize algorithm time complexity

  
