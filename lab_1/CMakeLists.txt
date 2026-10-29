cmake_minimum_required(VERSION 3.20)
project(lab_1 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_executable(lab_1 lab_1.cpp)
target_link_libraries(lab_1 PRIVATE Threads::Threads)
