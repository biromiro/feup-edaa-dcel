// Copyright 2023 EDAA Group C
#include <iostream>
#include "./dcel.hpp"
#include "auxiliary.hpp"

int main() {
    const auto dcel = parseJSONtoDCEL("countries-land-10km.geo.json");

    for (const auto& face: dcel->getFaces()){
        std::cout << face->getProperty() << std::endl;
    }

    return 0;
}
