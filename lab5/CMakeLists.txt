cmake_minimum_required(VERSION 3.23)
project(lab5)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
add_executable(lab5 main.cpp)

include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(lab5 ${OpenCV_LIBS})
