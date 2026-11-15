cmake_minimum_required(VERSION 3.20)
project(lab_5 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_executable(lab_5 lab_5.cpp MyMutex.cpp)
target_link_libraries(lab_5 PRIVATE Threads::Threads)