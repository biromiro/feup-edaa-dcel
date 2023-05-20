//
// Created by nrtc on 5/18/2023.
//

#ifndef DCEL_INTERSECTION_H
#define DCEL_INTERSECTION_H

#include <set>
#include "vertex.hpp"
#include "half_edge.hpp"
#include "GeographicPoint.h"

class Intersection {
public:
    explicit Intersection(GeographicPoint intersectionPoint);

    [[nodiscard]] const GeographicPoint &getIntersectionPoint() const;

    void addEdge(const std::shared_ptr<HalfEdge<GeographicPoint>>& edge);

    const std::set<std::shared_ptr<HalfEdge<GeographicPoint>>> &getEdges() const;

private:
    GeographicPoint intersectionPoint;
    std::set<std::shared_ptr<HalfEdge<GeographicPoint>>> edges;
};


#endif //DCEL_INTERSECTION_H
