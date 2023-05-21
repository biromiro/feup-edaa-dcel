//
// Created by nrtc on 5/18/2023.
//

#ifndef DCEL_INTERSECTION_H
#define DCEL_INTERSECTION_H

#include <set>
#include "../../../dcel_substructures/Vertex.hpp"
#include "../../../dcel_substructures/HalfEdge.hpp"
#include "../../../auxiliary/geographic_point/GeographicPoint.h"

class Intersection {
public:
    explicit Intersection(GeographicPoint intersectionPoint);

    [[nodiscard]] const GeographicPoint &getIntersectionPoint() const;

    void addEdge(const std::shared_ptr<HalfEdge<GeographicPoint>>& edge);

    [[nodiscard]] const std::set<std::shared_ptr<HalfEdge<GeographicPoint>>> &getEdges() const;

    [[nodiscard]] const std::set<std::pair<std::shared_ptr<HalfEdge<GeographicPoint>>, std::shared_ptr<HalfEdge<GeographicPoint>>>> &getEdgePairs() const;

    [[nodiscard]] const std::pair<std::shared_ptr<HalfEdge<GeographicPoint>>, std::shared_ptr<HalfEdge<GeographicPoint>>> &getLeftEdge() const;

    void setLeftEdge(const std::shared_ptr<HalfEdge<GeographicPoint>> &leftEdge);

private:
    GeographicPoint intersectionPoint;
    std::set<std::shared_ptr<HalfEdge<GeographicPoint>>> edges;
    std::set<std::pair<std::shared_ptr<HalfEdge<GeographicPoint>>, std::shared_ptr<HalfEdge<GeographicPoint>>>> edgePairs;
    std::pair<std::shared_ptr<HalfEdge<GeographicPoint>>, std::shared_ptr<HalfEdge<GeographicPoint>>> leftEdge;
};


#endif //DCEL_INTERSECTION_H
