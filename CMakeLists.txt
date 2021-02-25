cmake_minimum_required(VERSION 3.13)

project(openmp-workshop)

find_package(OpenMP)

add_executable(example_00 example_00.cpp)
target_link_libraries(example_00 OpenMP::OpenMP_CXX)
