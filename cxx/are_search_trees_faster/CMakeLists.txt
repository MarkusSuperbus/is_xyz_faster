cmake_minimum_required(VERSION 3.25.1)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-Wall -Werror")

project(benchmark-search-tree)
find_package(benchmark)

add_executable(main main.cc SearchTree.h ISearchTree.h StandardMapWrapper.h)
target_link_libraries(main benchmark::benchmark)
add_executable(test test.cc SearchTree.h ISearchTree.h StandardMapWrapper.h)
