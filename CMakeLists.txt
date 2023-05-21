cmake_minimum_required(VERSION 3.23)
project(dcel)
 
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#find_package(nlohmann_json REQUIRED)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} src/dcel/main.cpp src/dcel/auxiliary/geographic_point/GeographicPoint.cpp src/dcel/auxiliary/Auxiliary.cpp src/dcel/algorithms/line_sweep/LineSweep.cpp src/dcel/algorithms/line_sweep/event/Event.cpp src/dcel/auxiliary/parser/Parser.cpp src/dcel/auxiliary/parser/Parser.h src/dcel/algorithms/line_sweep/segment/Segment.cpp src/dcel/algorithms/line_sweep/segment/Segment.h src/dcel/algorithms/line_sweep/intersection/Intersection.cpp src/dcel/algorithms/line_sweep/intersection/Intersection.h src/dcel/algorithms/map_overlay/MapOverlay.cpp src/dcel/algorithms/map_overlay/MapOverlay.h)
#target_link_libraries(${PROJECT_NAME} nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME})

