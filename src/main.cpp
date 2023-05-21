// Copyright 2023 EDAA Group C
#include <matplot/matplot.h>

#include <iostream>
#include <string>

#include "dcel/algorithms/map_overlay/MapOverlay.h"
#include "dcel/auxiliary/parser/Parser.h"

int main() {
  const auto dcel1 = Parser::parseJSONtoDCEL("data/test-map-1.json");
  const auto dcel2 = Parser::parseJSONtoDCEL("data/test-map-2.json");
  const auto dcel3 = Parser::parseJSONtoDCEL("data/test-map-3.json");

  auto dcel = MapOverlay::overlayDCELs(dcel1, dcel2);
  // dcel = MapOverlay::overlayDCELs(dcel, dcel3);

  matplot::vector_1d xVals = {};
  matplot::vector_1d yVals = {};

  auto originEdge = dcel1->getEdges().begin()->get();

  xVals.push_back(originEdge->getOrigin()->getValue().getLongitude());
  yVals.push_back(originEdge->getOrigin()->getValue().getLatitude());

  auto nextEdge = originEdge->getNext();

  xVals.push_back(nextEdge->getOrigin()->getValue().getLongitude());
  yVals.push_back(nextEdge->getOrigin()->getValue().getLatitude());

  while (nextEdge->getOrigin() != originEdge->getOrigin()) {
    nextEdge = nextEdge->getNext();
    xVals.push_back(nextEdge->getOrigin()->getValue().getLongitude());
    yVals.push_back(nextEdge->getOrigin()->getValue().getLatitude());
  }

  std::string color = "r";
  matplot::fill(xVals, yVals, color);
  matplot::axis(matplot::equal);

  matplot::show();

  return 0;
}
