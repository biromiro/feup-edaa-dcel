//
// Created by nrtc on 4/17/2023.
//

#ifndef DCEL_AUXILIARY_HPP
#define DCEL_AUXILIARY_HPP

#include <string>

#include "dcel.hpp"
#include "GeographicPoint.h"
#include "vertex.hpp"

DCEL<GeographicPoint>* parseJSONtoDCEL(const std::string& jsonFilePath) {
    // read json file

    // create DCEL
    DCEL<GeographicPoint>* dcel = new DCEL<GeographicPoint>();
    // iterate through all vertices and create a vertex for each one
    /*for (auto vertex: vertices) {
        auto geovertex = new Vertex<GeographicPoint>(vertex);

    }*/

    return dcel;
}

#endif //DCEL_AUXILIARY_HPP
