cmake_minimum_required(VERSION 3.16.3)
project(lab7_avx)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
set(CMAKE_CXX_STANDARD 17)

add_executable(lab7_avx AVX_vectorization.cpp)

add_subdirectory(MatrixAVX)
target_include_directories(lab7_avx PUBLIC MatrixAVX)
target_link_libraries(lab7_avx PUBLIC matrix_avx_lib)
