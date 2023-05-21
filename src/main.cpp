// Copyright 2023 EDAA Group C
#include <iostream>

#include "dcel/auxiliary/parser/Parser.h"
#include "dcel/algorithms/map_overlay/MapOverlay.h"

int main() {
    const auto dcel1 = Parser::parseJSONtoDCEL("test-map-10.json");
    const auto dcel2 = Parser::parseJSONtoDCEL("test-map-11.json");
    const auto dcel3 = Parser::parseJSONtoDCEL("test-map-12.json");

    auto dcel = MapOverlay::overlayDCELs(dcel1, dcel2);
    dcel = MapOverlay::overlayDCELs(dcel, dcel3);

    return 0;
}
