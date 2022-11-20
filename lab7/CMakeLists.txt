cmake_minimum_required(VERSION 3.23)
project(lab7)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
set(CMAKE_CXX_STANDARD 17)

add_executable(lab7 disable_vectorization.cpp)
