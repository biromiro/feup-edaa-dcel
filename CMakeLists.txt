cmake_minimum_required(VERSION 3.23)
project(dcel)
 
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#find_package(nlohmann_json REQUIRED)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} src/dcel/main.cpp src/dcel/GeographicPoint.cpp src/dcel/auxiliary.cpp src/dcel/LineSweep.cpp src/dcel/Event.cpp src/dcel/Parser.cpp src/dcel/Parser.h src/dcel/Segment.cpp src/dcel/Segment.h src/dcel/Intersection.cpp src/dcel/Intersection.h)
#target_link_libraries(${PROJECT_NAME} nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME})

