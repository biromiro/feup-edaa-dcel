//
// Created by nrtc on 5/18/2023.
//

#include "Intersection.h"

#include <utility>

Intersection::Intersection(GeographicPoint intersectionPoint) :
        intersectionPoint(intersectionPoint)
        {}

const GeographicPoint &Intersection::getIntersectionPoint() const {
    return intersectionPoint;
}

void Intersection::addEdge(const std::shared_ptr<HalfEdge<GeographicPoint>>& edge) {
    edges.insert(edge);
}

const std::set<std::shared_ptr<HalfEdge<GeographicPoint>>> &Intersection::getEdges() const {
    return edges;
}
