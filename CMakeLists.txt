cmake_minimum_required(VERSION 3.23)
project(dcel)
 
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(nlohmann_json REQUIRED)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} src/dcel/main.cpp src/dcel/GeographicPoint.cpp src/dcel/auxiliary.hpp)
target_link_libraries(${PROJECT_NAME} nlohmann_json::nlohmann_json)
