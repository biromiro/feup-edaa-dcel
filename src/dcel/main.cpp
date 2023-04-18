// Copyright 2023 EDAA Group C
#include <iostream>
#include "./dcel.hpp"
#include "auxiliary.hpp"

int main() {
        const auto dcel = parseJSONtoDCEL("countries-land-10km.geo.json");

    for (const auto& vertex: dcel->getVertices()){
        std::cout << vertex->getValue() << std::endl;
    }

    return 0;
}
