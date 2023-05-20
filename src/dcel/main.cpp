// Copyright 2023 EDAA Group C
#include <iostream>

#include "LineSweep.h"
#include "Parser.h"

int main() {
  const auto dcel1 = Parser::parseJSONtoDCEL("../data/test-map-1.json");
  const auto dcel2 = Parser::parseJSONtoDCEL("../data/test-map-2.json");

  std::set<std::shared_ptr<HalfEdge<GeographicPoint>>> edges;

  for (const auto& edge : dcel1->getEdges()) edges.insert(edge);

  for (const auto& edge : dcel2->getEdges()) edges.insert(edge);

  std::cout << "Should have "
            << dcel1->getVertices().size() + dcel2->getVertices().size()
            << " vertices." << std::endl;

  const auto intersections = LineSweep::findIntersections(edges);

  std::cout << "The found intersection points were:" << std::endl;

  for (const auto& intersection : intersections) {
    auto vertex = dcel1->getVertex(intersection->getIntersectionPoint());
    auto vertex2 = dcel2->getVertex(intersection->getIntersectionPoint());
    if (!vertex && !vertex2)
      std::cout << intersection->getIntersectionPoint() << std::endl;
  }

  return 0;
}
