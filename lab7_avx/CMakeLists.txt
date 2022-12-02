cmake_minimum_required(VERSION 3.16.3)
project(lab7_sse2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
set(CMAKE_CXX_STANDARD 17)

add_executable(lab7_sse2 AVX_vectorization.cpp)

add_subdirectory(MatrixAVX)
target_include_directories(lab7_sse2 PUBLIC MatrixAVX)
target_link_libraries(lab7_sse2 PUBLIC matrix_avx_lib)
