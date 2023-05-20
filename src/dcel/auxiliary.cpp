//
// Created by nrtc on 4/17/2023.
//

#include <cmath>
#include <iostream>
#include "./auxiliary.h"

using json = nlohmann::json;

bool approximatelyEqual(double a, double b)
{
    return std::fabs(a - b) <= ( (std::fabs(a) < std::fabs(b) ? std::fabs(b) : std::fabs(a)) * EPSILON);
}

bool isClockwise(
    const std::vector<std::shared_ptr<Vertex<GeographicPoint>>>& vertices) {
  double accumulator = 0;
  for (auto i = 0; i < vertices.size(); i++) {
    auto currentPoint = vertices.at(i)->getValue();
    auto nextPoint = vertices.at((i + 1) % vertices.size())->getValue();

    accumulator += (nextPoint.getLatitude() - currentPoint.getLatitude()) *
                   (nextPoint.getLongitude() + currentPoint.getLongitude());
  }
  return accumulator > 0;
}

bool ccw (GeographicPoint p1, GeographicPoint p2, GeographicPoint p3) {
    return (p3.getLatitude()-p1.getLatitude()) * (p2.getLongitude()-p1.getLongitude()) >
           (p2.getLatitude()-p1.getLatitude()) * (p3.getLongitude()-p1.getLongitude());
}

bool edgesIntersect (
        const std::shared_ptr<HalfEdge<GeographicPoint>>& edge1,
        const std::shared_ptr<HalfEdge<GeographicPoint>>& edge2
) {
    auto p1 = edge1->getOrigin()->getValue();
    auto p2 = edge1->getTwin()->getOrigin()->getValue();
    auto p3 = edge2->getOrigin()->getValue();
    auto p4 = edge2->getTwin()->getOrigin()->getValue();

    return ccw(p1,p3,p4) != ccw(p2,p3,p4) && ccw(p1,p2,p3) != ccw(p1,p2,p4);
}

GeographicPoint calculateIntersectionPoint(
        const std::shared_ptr<HalfEdge<GeographicPoint>>& edge1,
        const std::shared_ptr<HalfEdge<GeographicPoint>>& edge2
) {
    if (!edgesIntersect(edge1, edge2))
        throw std::invalid_argument("Can't generate intersection point since edges do not intersect!");

    auto p1 = edge1->getOrigin()->getValue();
    auto p2 = edge1->getTwin()->getOrigin()->getValue();
    auto p3 = edge2->getOrigin()->getValue();
    auto p4 = edge2->getTwin()->getOrigin()->getValue();

    auto x1 = p1.getLatitude(), y1 = p1.getLongitude(),
            x2 = p2.getLatitude(), y2 = p2.getLongitude(),
            x3 = p3.getLatitude(), y3 = p3.getLongitude(),
            x4 = p4.getLatitude(), y4 = p4.getLongitude();

    return {
            ((x1*y2 - y1*x2)*(x3 - x4) - (x1 - x2)*(x3*y4 - y3*x4)) / ((x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4)),
            ((x1*y2 - y1*x2)*(y3 - y4) - (y1 - y2)*(x3*y4 - y3*x4)) / ((x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4))
    };
}