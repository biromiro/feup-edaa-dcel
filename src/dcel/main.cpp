// Copyright 2023 EDAA Group C
#include <iostream>

#include "Parser.h"
#include "LineSweep.h"
#include "MapOverlay.h"

int main() {
    const auto dcel1 = Parser::parseJSONtoDCEL("test-map-1.json");
    const auto dcel2 = Parser::parseJSONtoDCEL("test-map-2.json");

    auto dcel = MapOverlay::overlayDCELs(dcel1, dcel2);

    return 0;
}
