cmake_minimum_required(VERSION 3.23)
project(dcel)

find_package(nlohmann_json REQUIRED)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} src/dcel/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
