cmake_minimum_required(VERSION 3.20)
project(lab_4 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_executable(lab_4 lab_4.cpp)
target_link_libraries(lab_4 PRIVATE Threads::Threads)