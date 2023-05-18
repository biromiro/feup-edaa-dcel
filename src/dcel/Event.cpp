//
// Created by nrtc on 5/17/2023.
//

#include "Event.h"

Event::Event(const GeographicPoint& endpoint) {

    this->edges = std::set<std::shared_ptr<HalfEdge<GeographicPoint>>>();
    /*
    if (type == INTERSECTION) {
        if (!edge2)
            throw std::invalid_argument("Intersection event must have two edges as constructor.");

        this->endpoint = calculateIntersectionPoint(edge, edge2);
        edges.insert(edge);
        edges.insert(edge2);
        return;
    }

    auto orig = edge->getOrigin()->getValue();
    auto dest = edge->getTwin()->getOrigin()->getValue();

    if (type == UPPER_ENDPOINT)
        this->endpoint = orig < dest ? orig : dest;
    else if (type == LOWER_ENDPOINT)
        this->endpoint = orig < dest ? dest : orig;

    edges.insert(edge);*/
    this->endpoint = endpoint;
}

bool Event::operator<(const Event &rhs) const {
    return this->endpoint < rhs.endpoint;
}

void Event::addEdge(const std::shared_ptr<HalfEdge<GeographicPoint>> &edge) {
    (this->edges).insert(edge);
}

const GeographicPoint &Event::getEndpoint() const {
    return endpoint;
}

const std::set<std::shared_ptr<HalfEdge<GeographicPoint>>> &Event::getEdges() const {
    return edges;
}

