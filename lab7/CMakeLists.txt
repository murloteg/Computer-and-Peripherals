cmake_minimum_required(VERSION 3.16.3)
project(lab7)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
set(CMAKE_CXX_STANDARD 17)

add_executable(lab7 disable_vectorization.cpp)
#add_executable(lab7 AVX_vectorization.cpp)

add_subdirectory(Matrix)
target_include_directories(lab7 PUBLIC Matrix)
target_link_libraries(lab7 PUBLIC matrix_lib)