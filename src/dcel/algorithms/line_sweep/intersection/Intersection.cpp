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
    edgePairs.insert(std::make_pair(edge, edge->getTwin()));
}

const std::set<std::shared_ptr<HalfEdge<GeographicPoint>>> &Intersection::getEdges() const {
    return edges;
}

const std::set<std::pair<std::shared_ptr<HalfEdge<GeographicPoint>>, std::shared_ptr<HalfEdge<GeographicPoint>>>> &Intersection::getEdgePairs() const {
    return edgePairs;
}

const std::pair<std::shared_ptr<HalfEdge<GeographicPoint>>, std::shared_ptr<HalfEdge<GeographicPoint>>> &Intersection::getLeftEdge() const {
    return leftEdge;
}

void Intersection::setLeftEdge(const std::shared_ptr<HalfEdge<GeographicPoint>> &leftEdge) {
    this->leftEdge = std::make_pair(leftEdge, leftEdge->getTwin());
}
