cmake_minimum_required(VERSION 3.13)

project(openmp-workshop)

find_package(OpenMP REQUIRED)

file(GLOB FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp")
foreach (FILE ${FILES})
    get_filename_component(EXECUTABLE_NAME ${FILE} NAME_WE)
    add_executable(${EXECUTABLE_NAME} ${FILE})
    target_link_libraries(${EXECUTABLE_NAME} OpenMP::OpenMP_CXX)
    message("-- Added example ${EXECUTABLE_NAME}")
endforeach ()
