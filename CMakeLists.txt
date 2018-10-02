cmake_minimum_required(VERSION 3.10)

project(krazymeans)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast -march=native")

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/utils/RandomGenerator.hpp
        src/utils/Timer.hpp
        src/utils/FeatureVec.cpp src/utils/FeatureVec.hpp
        src/utils/DataSet.cpp src/utils/DataSet.hpp
        src/krazy/KrazyMeans.cpp src/krazy/KrazyMeans.hpp)
