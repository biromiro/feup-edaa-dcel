// Copyright 2023 EDAA Group C
#include <matplot/matplot.h>

#include <iostream>
#include <string>

#include "dcel/algorithms/map_overlay/MapOverlay.h"
#include "dcel/auxiliary/parser/Parser.h"
#include "matplot/freestanding/plot.h"
#include "visualization/Visualization.h"

#include <filesystem>

using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

int main() {
    auto dcel = Parser::parseJSONtoDCEL("../data/usdata/nation/US.geojson");

    for (const auto& dirEntry : recursive_directory_iterator("../data/usdata/region")) {
        auto dcel1 = Parser::parseJSONtoDCEL(dirEntry.path().string());
        dcel = MapOverlay::overlayDCELs(dcel, dcel1);
    }

    /*for (const auto& dirEntry : recursive_directory_iterator("../data/usdata/state")) {
        auto dcel1 = Parser::parseJSONtoDCEL(dirEntry.path().string());
        dcel = MapOverlay::overlayDCELs(dcel, dcel1);
    }*/

  int i = 1;
  for (const auto& face : dcel->getFaces()) {
    std::cout << "This is face " << i++
              << " and has the following properties:" << std::endl;
    std::cout << face->getProperty() << std::endl;

    if (face != dcel->getUnboundedFace()) {
      std::cout << "The face follows the following path:" << std::endl;
      auto firstEdge = face->getOuter();
      std::shared_ptr<HalfEdge<GeographicPoint>> currentEdge = firstEdge;
      do {
        std::cout << currentEdge->getOrigin()->getValue() << std::endl;
        currentEdge = currentEdge->getNext();
      } while (currentEdge != firstEdge);
    } else {
      std::cout << "It is the unbounded face. It has no properties nor path to "
                   "follow."
                << std::endl;
    }
  }

  auto visualizer = Visualizer(dcel);
  visualizer.plotDCEL(matplot::on);

  // visualizer.setDCEL(dcel2);

  // visualizer.plotDCEL(matplot::off);

  visualizer.showDCEL();

  return 0;
}
