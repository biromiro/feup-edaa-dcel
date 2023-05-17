//
// Created by nrtc on 5/17/2023.
//

#include "Event.h"

#include <utility>

Event::Event(EventType type, const std::shared_ptr<HalfEdge<GeographicPoint>> &edge,
             const std::shared_ptr<HalfEdge<GeographicPoint>> &edge2): type(type), edge(edge) {

    if (type == INTERSECTION) {
        if (!edge2)
            throw std::invalid_argument("Intersection event must have two edges as constructor.");


        return;
    }

    auto orig = edge->getOrigin()->getValue();
    auto dest = edge->getTwin()->getOrigin()->getValue();

    if (type == UPPER_ENDPOINT)
        this->endpoint = orig;
    else if (type == LOWER_ENDPOINT)
        this->endpoint = dest;
}

EventType Event::getType() const {
    return type;
}

bool Event::operator<(const Event &rhs) const {
    return this->endpoint < rhs.endpoint;
}

bool Event::operator>(const Event &rhs) const {
    return rhs < *this;
}

bool Event::operator<=(const Event &rhs) const {
    return !(rhs < *this);
}

bool Event::operator>=(const Event &rhs) const {
    return !(*this < rhs);
}


