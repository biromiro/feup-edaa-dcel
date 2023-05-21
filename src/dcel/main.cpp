// Copyright 2023 EDAA Group C
#include <iostream>

#include "auxiliary/parser/Parser.h"
#include "map_overlay/MapOverlay.h"

int main() {
    const auto dcel1 = Parser::parseJSONtoDCEL("test-map-10.json");
    const auto dcel2 = Parser::parseJSONtoDCEL("test-map-11.json");

    auto dcel = MapOverlay::overlayDCELs(dcel1, dcel2);

    return 0;
}
