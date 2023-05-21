// Copyright 2023 EDAA Group C
#include <iostream>

#include "dcel/auxiliary/parser/Parser.h"
#include "dcel/algorithms/map_overlay/MapOverlay.h"

int main() {
    const auto dcel1 = Parser::parseJSONtoDCEL("test-map-1.json");
    const auto dcel2 = Parser::parseJSONtoDCEL("test-map-2.json");
    //const auto dcel3 = Parser::parseJSONtoDCEL("test-map-12.json");

    auto dcel = MapOverlay::overlayDCELs(dcel1, dcel2);
    //dcel = MapOverlay::overlayDCELs(dcel, dcel3);

    int i = 1;
    for (const auto& face: dcel->getFaces()) {
        std::cout << "This is face " << i++ << " and has the following properties:" << std::endl;
        std::cout << face->getProperty() << std::endl;

        if (face != dcel->getUnboundedFace()) {
            std::cout << "The face follows the following path:" << std::endl;
            auto firstEdge = face->getOuter();
            std::shared_ptr<HalfEdge<GeographicPoint>> currentEdge = firstEdge;
            do {
                std::cout << currentEdge->getOrigin()->getValue() << std::endl;
                currentEdge = currentEdge->getNext();
            } while (currentEdge != firstEdge);
        } else std::cout << "It is the unbounded face. It has no properties nor path to follow." << std::endl;
    }

    return 0;
}
