cmake_minimum_required(VERSION 3.23)
project(dcel)

set(CMAKE_CXX_STANDARD 20)

add_executable(dcel main.cpp vertex.cpp half_edge.cpp face.cpp dcel.cpp)
