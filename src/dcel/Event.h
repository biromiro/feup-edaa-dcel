//
// Created by nrtc on 5/17/2023.
//

#ifndef DCEL_EVENT_H
#define DCEL_EVENT_H

#include <iostream>
#include "./half_edge.hpp"
#include "./vertex.hpp"
#include "GeographicPoint.h"

// TODO: Should store the endpoint and have a function that returns a ordered value or overload operator
//       to order events

enum EventType {
    UPPER_ENDPOINT,
    LOWER_ENDPOINT,
    INTERSECTION
};

class Event {
public:
    Event(EventType type, const std::shared_ptr<HalfEdge<GeographicPoint>>& edge,
          const std::shared_ptr<HalfEdge<GeographicPoint>>& edge2 = nullptr);
    [[nodiscard]] EventType getType() const;

    bool operator<(const Event &rhs) const;

    bool operator>(const Event &rhs) const;

    bool operator<=(const Event &rhs) const;

    bool operator>=(const Event &rhs) const;

private:
    EventType type;
    std::shared_ptr<HalfEdge<GeographicPoint>> edge;
    GeographicPoint endpoint;
    bool isDownwards;

};


#endif //DCEL_EVENT_H
