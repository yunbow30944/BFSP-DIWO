cmake_minimum_required(VERSION 3.29)
project(project)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(project
    Alg1.cpp
    Alg1.h
    Alg2.hpp
    GlobalData.cpp
    GlobalData.h
    main.cpp
    Utils.cpp
    Utils.h)
