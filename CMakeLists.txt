cmake_minimum_required(VERSION 3.23)
project(dcel)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(matplotplusplus)

set(CMAKE_CXX_STANDARD 20)

add_executable(
  ${PROJECT_NAME}
  src/main.cpp
  src/dcel/json.hpp
  src/dcel/dcel.tpp
  src/dcel/dcel_substructures/Face.tpp
  src/dcel/dcel_substructures/HalfEdge.tpp
  src/dcel/dcel_substructures/Vertex.tpp
  src/dcel/auxiliary/Auxiliary.cpp
  src/dcel/auxiliary/parser/Parser.cpp
  src/dcel/auxiliary/geographic_point/GeographicPoint.cpp
  src/dcel/auxiliary/cycle_graph/Cycle.cpp
  src/dcel/auxiliary/cycle_graph/connectivity/Connectivity.tpp
  src/dcel/auxiliary/cycle_graph/auxiliary/Edge.tpp
  src/dcel/auxiliary/cycle_graph/auxiliary/Graph.tpp
  src/dcel/auxiliary/cycle_graph/auxiliary/Node.tpp
  src/dcel/algorithms/map_overlay/MapOverlay.cpp
  src/dcel/algorithms/line_sweep/LineSweep.cpp
  src/dcel/algorithms/line_sweep/segment/Segment.cpp
  src/dcel/algorithms/line_sweep/intersection/Intersection.cpp
  src/dcel/algorithms/line_sweep/event/Event.cpp
  src/visualization/Visualization.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC matplot)
