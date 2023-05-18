// Copyright 2023 EDAA Group C
#include <iostream>

#include "Parser.h"
#include "LineSweep.h"

int main() {
    const auto dcel1 = Parser::parseJSONtoDCEL("test-map-1.json");
    const auto dcel2 = Parser::parseJSONtoDCEL("test-map-2.json");

    for (const auto& face : dcel1->getFaces()) {
    std::cout << face->getProperty() << std::endl;
    }

    std::set<std::shared_ptr<HalfEdge<GeographicPoint>>> edges;

    for (const auto& edge: dcel1->getEdges())
        edges.insert(edge);

    for (const auto& edge: dcel2->getEdges())
        edges.insert(edge);

    const auto intersections = LineSweep::findIntersections(edges);

    std::cout << "The found intersection points were:" << std::endl;

    for (const auto& intersection: intersections) {
      std::cout << intersection.getIntersectionPoint() << std::endl;
    }

    return 0;
}
