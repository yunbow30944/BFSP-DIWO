cmake_minimum_required(VERSION 3.29)
project(project)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(project
    Alg1.cpp
    Alg1.h
    Alg2.cpp
    Alg2.h
    Alg3.cpp
    Alg3.h
    Alg4.cpp
    Alg4.h
    GlobalData.cpp
    GlobalData.h
    main.cpp
    Utils.cpp
    Utils.h
)
