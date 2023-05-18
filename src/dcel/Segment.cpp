//
// Created by nrtc on 5/18/2023.
//

#include "Segment.h"

Segment::Segment(const std::shared_ptr<HalfEdge<GeographicPoint>> &edge) {
    this->edge = edge;
    this->vertex1 = edge->getOrigin()->getValue();
    this->vertex2 = edge->getTwin()->getOrigin()->getValue();
    this->slope = (vertex2.getLatitude() - vertex1.getLatitude()) / (vertex2.getLongitude() - vertex1.getLongitude());
    this->bias = vertex1.getLatitude() - this->slope * vertex1.getLongitude();
}

GeographicPoint Segment::getUpperEndpoint() const {
    return vertex1 < vertex2 ? vertex1 : vertex2;
}

GeographicPoint Segment::getLowerEndpoint() const {
    return vertex1 < vertex2 ? vertex2 : vertex1;
}

double Segment::getSegmentCurrentLongitude(double latitude) const {
    return (latitude - this->bias) / this->slope;
}

const std::shared_ptr<HalfEdge<GeographicPoint>> &Segment::getEdge() const {
    return edge;
}

bool Segment::operator<(const Segment &rhs) const {
    if (this->getUpperEndpoint() == rhs.getUpperEndpoint())
        return this->getLowerEndpoint() < rhs.getLowerEndpoint();
    return this->getUpperEndpoint() < rhs.getUpperEndpoint();
}

bool Segment::operator==(const Segment &rhs) const {
    return (vertex1 == rhs.vertex1 && vertex2 == rhs.vertex2) ||
    (vertex1 == rhs.vertex2 && vertex2 == rhs.vertex1);
}

bool Segment::operator!=(const Segment &rhs) const {
    return !(rhs == *this);
}

const GeographicPoint &Segment::getVertex1() const {
    return vertex1;
}

const GeographicPoint &Segment::getVertex2() const {
    return vertex2;
}

std::ostream &operator<<(std::ostream &os, const Segment &segment) {
    os << "edge: " << segment.edge << " vertex1: " << segment.vertex1 << " vertex2: " << segment.vertex2 << " slope: "
       << segment.slope << " bias: " << segment.bias;
    return os;
}
