cmake_minimum_required(VERSION 3.8)
project(poissonDisk)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp PoissonDisk2D.h LoadObj.h PoissonDisk3D.h)
add_executable(poissonDisk ${SOURCE_FILES})