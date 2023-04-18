//
// Created by nrtc on 4/17/2023.
//

#ifndef DCEL_AUXILIARY_HPP
#define DCEL_AUXILIARY_HPP

#include <string>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

#include "dcel.hpp"
#include "GeographicPoint.h"
#include "vertex.hpp"

using json = nlohmann::json;

void genGeographicPoints(const std::shared_ptr<DCEL<GeographicPoint>>& dcel, const json& json_file) {

    std::shared_ptr<Vertex<GeographicPoint>> firstPoint = nullptr;
    std::shared_ptr<HalfEdge<GeographicPoint>>  firstEdge1 = nullptr,
                                                firstEdge2 = nullptr,
                                                previousEdge1 = nullptr,
                                                previousEdge2 = nullptr;

    for (auto coords : json_file[0]) {
        double latitude = coords[0], longitude = coords[1];

        auto geoPoint = GeographicPoint(latitude, longitude);
        auto vertex = std::make_shared<Vertex<GeographicPoint>>(geoPoint);
        dcel->addVertex(vertex);

        if (firstPoint == nullptr) {
            firstPoint = vertex;
            firstEdge1 = std::make_shared<HalfEdge<GeographicPoint>>();
            firstEdge2 = std::make_shared<HalfEdge<GeographicPoint>>();

            dcel->addEdge(firstEdge1);
            dcel->addEdge(firstEdge2);

            firstEdge1->setTwin(firstEdge2);
            firstEdge2->setTwin(firstEdge1);

            firstEdge1->setOrigin(vertex);

            previousEdge1 = firstEdge1;
            previousEdge2 = firstEdge2;
            continue;
        }

        auto edge1 = std::make_shared<HalfEdge<GeographicPoint>>();
        auto edge2 = std::make_shared<HalfEdge<GeographicPoint>>();

        dcel->addEdge(edge1);
        dcel->addEdge(edge2);

        edge1->setOrigin(vertex);
        previousEdge2->setOrigin(vertex);

        edge1->setTwin(edge2);
        edge2->setTwin(edge1);

        previousEdge1->setNext(edge1);
        edge2->setNext(previousEdge2);

        edge1->setPrev(previousEdge1);
        previousEdge2->setPrev(edge2);
    }

    firstEdge1->setPrev(previousEdge1);
    previousEdge1->setNext(firstEdge1);

    previousEdge2->setOrigin(firstPoint);

    previousEdge2->setPrev(firstEdge2);
    firstEdge2->setNext(previousEdge2);
}

std::shared_ptr<DCEL<GeographicPoint>> parseJSONtoDCEL(const std::string& jsonFilePath) {
    // read json file
    std::ifstream f("../data/" + jsonFilePath);
    json data = json::parse(f);

    // create DCEL
    auto dcel = std::make_shared<DCEL<GeographicPoint>>();

    if (data["type"] != "FeatureCollection"){
        const std::string error_msg = "The provided json file does not have the expected type 'FeatureCollection'.";
        throw std::invalid_argument(error_msg);
    }

    // iterate through all vertices and create a vertex for each one
    for (json::iterator it = data["features"].begin(); it != data["features"].end(); ++it) {
        auto feat = *it;

        if (feat["type"] != "Feature") {
            const std::string error_msg = "The provided json file does not have the expected type 'Feature'.";
            throw std::invalid_argument(error_msg);
        }

        auto geometry = feat["geometry"];

        if (geometry["type"] != "MultiPolygon" and geometry["type"] != "Polygon"){
            const std::string error_msg = "The provided json file does not have the expected type 'Polygon' or 'MultiPolygon'.";
            throw std::invalid_argument(error_msg);
        }

        auto coords = geometry["coordinates"];
        if (geometry["type"] == "MultiPolygon") {
            for (auto & coord : coords) {
                genGeographicPoints(dcel, coord);
            }
        } else if (geometry["type"] == "Polygon") {
            genGeographicPoints(dcel, coords);
        }

    }

    return dcel;
}

#endif //DCEL_AUXILIARY_HPP
