// Copyright 2023 EDAA Group C
#include <iostream>

#include "Parser.h"
#include "LineSweep.h"

int main() {
  const auto dcel = Parser::parseJSONtoDCEL("countries-land-10km.geo.json");

  for (const auto& face : dcel->getFaces()) {
    std::cout << face->getProperty() << std::endl;
  }

  const auto intersections = LineSweep::findIntersections(dcel->getEdges());

  return 0;
}
