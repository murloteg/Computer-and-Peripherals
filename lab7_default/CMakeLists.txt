cmake_minimum_required(VERSION 3.16.3)
project(lab7_default)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
set(CMAKE_CXX_STANDARD 17)

add_executable(lab7_default disable_vectorization.cpp)

add_subdirectory(MatrixDefault)
target_include_directories(lab7_default PUBLIC MatrixDefault)
target_link_libraries(lab7_default PUBLIC matrix_default_lib)
