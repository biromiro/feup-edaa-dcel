//
// Created by nrtc on 5/17/2023.
//

#ifndef DCEL_EVENT_H
#define DCEL_EVENT_H

#include <iostream>
#include "./half_edge.hpp"
#include "./vertex.hpp"
#include "auxiliary.h"
#include "GeographicPoint.h"

// TODO: Should store the endpoint and have a function that returns a ordered value or overload operator
//       to order events

class Event {
public:
    explicit Event(const GeographicPoint& endpoint);
    [[nodiscard]] const GeographicPoint &getEndpoint() const;

    void addEdge(const std::shared_ptr<HalfEdge<GeographicPoint>>& edge);

    [[nodiscard]] const std::set<std::shared_ptr<HalfEdge<GeographicPoint>>> &getEdges() const;

    bool operator<(const Event &rhs) const;


private:
    std::set<std::shared_ptr<HalfEdge<GeographicPoint>>> edges;
    GeographicPoint endpoint;
};


#endif //DCEL_EVENT_H
