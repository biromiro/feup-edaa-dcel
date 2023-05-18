// Copyright (c) 2023 EDAA Group C. All Rights Reserved.
#pragma once

#include <iostream>
#include <queue>
#include <vector>

#include "./half_edge.hpp"
#include "./intersection.hpp"
#include "auxiliary.h"
#include "GeographicPoint.h"
#include "Event.h"

class Segment{
public:

    Segment(const std::shared_ptr<HalfEdge<GeographicPoint>>& edge);
    [[nodiscard]] GeographicPoint getUpperEndpoint() const;

    [[nodiscard]] GeographicPoint getLowerEndpoint() const;

    [[nodiscard]] const std::shared_ptr<HalfEdge<GeographicPoint>> &getEdge() const;
    [[nodiscard]] double getSegmentCurrentLongitude(double latitude) const;

private:
    std::shared_ptr<HalfEdge<GeographicPoint>> edge;
    GeographicPoint vertex1;
    GeographicPoint vertex2;
    double slope;
    double bias;

};


class LineSweep {
public:

    static void insertInStatusTree(
            std::vector<Segment>& statusTree,
            const Event& edge
            );

    static void removeFromStatusTree(
            std::vector<Segment>& statusTree,
            const Event& edge
    );

    static void swapWithNeighborInStatusTree(
            std::vector<Segment>& statusTree,
            const Event& edge
    );


    static std::vector<Intersection<GeographicPoint>> findIntersections(
            const std::set<std::shared_ptr<HalfEdge<GeographicPoint>>>& edges);

    static void handleEventPoint(
            const Event& event,
            std::priority_queue<Event>& eventQ,
            std::vector<Segment>& statusTree
    );

    static void findNewEvent(
            const std::shared_ptr<HalfEdge<GeographicPoint>>& sl,
            const std::shared_ptr<HalfEdge<GeographicPoint>>& sr,
            const Event& p,
            std::priority_queue<Event>& eventQ
    );
};


