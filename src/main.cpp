// Copyright 2023 EDAA Group C
#include <matplot/matplot.h>

#include <iostream>
#include <string>

#include "dcel/algorithms/map_overlay/MapOverlay.h"
#include "dcel/auxiliary/parser/Parser.h"
#include "matplot/freestanding/plot.h"
#include "visualization/Visualizaion.h"

int main() {
  const auto dcel1 = Parser::parseJSONtoDCEL("data/hospital-feup.json");
  const auto dcel2 = Parser::parseJSONtoDCEL("data/fadeup-feup.json");

  auto dcel = MapOverlay::overlayDCELs(dcel1, dcel2);

  auto visualizer = Visualizer(dcel);
  visualizer.plotDCEL(matplot::on);

  // visualizer.setDCEL(dcel2);

  // visualizer.plotDCEL(matplot::off);

  visualizer.showDCEL();

  return 0;
}
