// Copyright 2023 EDAA Group C
#include <iostream>

#include "Parser.h"
#include "LineSweep.h"
#include "MapOverlay.h"

int main() {
    const auto dcel1 = Parser::parseJSONtoDCEL("test-map-5.json");
    const auto dcel2 = Parser::parseJSONtoDCEL("test-map-9.json");

    auto dcel = MapOverlay::overlayDCELs(dcel1, dcel2);

    return 0;
}
