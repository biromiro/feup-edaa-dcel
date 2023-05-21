#pragma once

#include <memory>
#include "../json.hpp"
#include <string>
#include <vector>

#include "../dcel.tpp"
#include "../dcel_substructures/HalfEdge.tpp"

#define EPSILON 1E-10

class GeographicPoint;

using json = nlohmann::json;

bool approximatelyEqual(double a, double b);

bool isClockwise(
    const std::vector<std::shared_ptr<Vertex<GeographicPoint>>>& vertices);

double distance(GeographicPoint a, GeographicPoint b);

bool isBetween(GeographicPoint a, GeographicPoint c, GeographicPoint b);

bool edgesIntersect (
        const std::shared_ptr<HalfEdge<GeographicPoint>>& edge1,
        const std::shared_ptr<HalfEdge<GeographicPoint>>& edge2
);

GeographicPoint calculateIntersectionPoint(
        const std::shared_ptr<HalfEdge<GeographicPoint>>& edge1,
        const std::shared_ptr<HalfEdge<GeographicPoint>>& edge2
        );