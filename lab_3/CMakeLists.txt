cmake_minimum_required (VERSION 3.20)

project(lab_3 CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)

add_executable(lab_3 lab_3.cpp)

target_link_libraries(lab_3 PRIVATE Threads::Threads)