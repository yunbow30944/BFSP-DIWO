cmake_minimum_required(VERSION 3.29)
project(project)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

add_executable(project
        src/Alg1.cpp
        src/Alg1.h
        src/Alg2.cpp
        src/Alg2.h
        src/Alg3.cpp
        src/Alg3.h
        src/Alg4.cpp
        src/Alg4.h
        src/GlobalData.cpp
        src/GlobalData.h
        src/main.cpp
        src/Utils.cpp
        src/Utils.h
        src/IO.cpp
        src/IO.h
        src/Test.cpp
        src/Test.h
)
