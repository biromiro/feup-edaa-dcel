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

std::shared_ptr<HalfEdge<GeographicPoint>> genGeographicPoints(const std::shared_ptr<DCEL<GeographicPoint>>& dcel,
                                                               const json& json_file) {

    std::shared_ptr<HalfEdge<GeographicPoint>>  edgeOfFace = nullptr;

    std::vector<std::pair<bool, std::shared_ptr<Vertex<GeographicPoint>>>> vertices; // vector of pairs <existed, vertex>
    std::vector<std::pair<std::shared_ptr<HalfEdge<GeographicPoint>>, std::shared_ptr<HalfEdge<GeographicPoint>>>> halfEdges;

    GeographicPoint firstGeoPoint = GeographicPoint(0,0);
    bool first = true;

    for (auto coords : json_file[0]) {
        double latitude = coords[0], longitude = coords[1];

        auto geoPoint = GeographicPoint(latitude, longitude);
        if (first){
            firstGeoPoint = geoPoint;
            first = false;
        } else if (geoPoint == firstGeoPoint) {
            break;
        }

        auto vertex = dcel->getVertex(geoPoint);

        if ( vertex == nullptr) {
            vertex = std::make_shared<Vertex<GeographicPoint>>(geoPoint);
            dcel->addVertex(vertex);
            vertices.emplace_back(false, vertex);
        } else vertices.emplace_back(true, vertex);

    }

    for (auto i = 0; i < vertices.size(); i++) {
        auto& [existed1, currentVertex] = vertices.at(i);
        auto& [existed2, nextVertex] = vertices.at((i + 1) % vertices.size());

        std::shared_ptr<HalfEdge<GeographicPoint>> halfEdge1, halfEdge2;

        if (existed1 && existed2){
            halfEdge1 = dcel->getHalfEdge(currentVertex, nextVertex);
            halfEdge2 = dcel->getHalfEdge(nextVertex, currentVertex);

            if (halfEdge1) {
                halfEdges.emplace_back(halfEdge1, halfEdge2);
                continue;
            }

            existed1 = false;
            existed2 = false;
        }

        halfEdge1 = std::make_shared<HalfEdge<GeographicPoint>>();
        halfEdge2 = std::make_shared<HalfEdge<GeographicPoint>>();

        dcel->addEdge(halfEdge1);
        dcel->addEdge(halfEdge2);

        halfEdge1->setOrigin(currentVertex);
        halfEdge2->setOrigin(nextVertex);

        halfEdge1->setTwin(halfEdge2);
        halfEdge2->setTwin(halfEdge1);

        halfEdges.emplace_back(halfEdge1, halfEdge2);
    }

    for (auto i=0; i < halfEdges.size(); i++) {

        auto& [previousHalfEdge1, previousHalfEdge2] = halfEdges.at(i);
        auto& [halfEdge1, halfEdge2] = halfEdges.at((i + 1) % halfEdges.size());
        auto& [nextHalfEdge1, nextHalfEdge2] = halfEdges.at((i + 2) % halfEdges.size());

        auto& [existed1, currentVertex] = vertices.at((i + 1) % vertices.size());
        auto& [existed2, nextVertex] = vertices.at((i + 2) % vertices.size());

        if (!currentVertex->getIncident())
            currentVertex->setIncident(halfEdge1);

        if (existed1 && existed2) {
            if (halfEdge1->getIncident()) {
                halfEdge2->setNext(previousHalfEdge2);
                halfEdge2->setPrev(nextHalfEdge2);
                edgeOfFace = halfEdge2;
            } else {
                halfEdge1->setNext(nextHalfEdge1);
                halfEdge1->setPrev(previousHalfEdge1);
                edgeOfFace = halfEdge1;
            }
            continue;
        }

        halfEdge1->setNext(nextHalfEdge1);
        halfEdge2->setNext(previousHalfEdge2);

        halfEdge1->setPrev(previousHalfEdge1);
        halfEdge2->setPrev(nextHalfEdge2);

    }

    return edgeOfFace != nullptr ? edgeOfFace : halfEdges.at(0).first;
}

void genFace(const std::shared_ptr<DCEL<GeographicPoint>>& dcel,
             const std::shared_ptr<HalfEdge<GeographicPoint>>& edge,
             const json& properties) {

    auto face = std::make_shared<Face<GeographicPoint>>(properties);
    dcel->addFace(face);
    face->setOuter(edge);

    auto currentEdge = (edge->getIncident() == nullptr ? edge : edge->getTwin());

    do {
        currentEdge->setIncident(face);
        currentEdge = currentEdge->getNext();
    } while (currentEdge != edge);

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
    std::cout << data["features"].size() << " features to parse." << std::endl;
    auto count = 1;
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
            auto counter = 1;
            for (auto & coord : coords) {
                auto edge = genGeographicPoints(dcel, coord);
                auto properties = feat["properties"];
                properties.emplace("MultiPolygonNumber", counter++);
                genFace(dcel, edge, properties);

            }

        } else if (geometry["type"] == "Polygon") {
            auto edge = genGeographicPoints(dcel, coords);
            genFace(dcel, edge, feat["properties"]);
        }

        std::cout << "Finished feature " << count++ << std::endl;

    }

    return dcel;
}

#endif //DCEL_AUXILIARY_HPP
