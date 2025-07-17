cmake_minimum_required(VERSION 3.25.1)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(benchmark-emplace-back)
find_package(benchmark)

add_executable(main main.cc)
target_link_libraries(main benchmark::benchmark)
add_executable(main2 main2.cc)
target_link_libraries(main2 benchmark::benchmark)
