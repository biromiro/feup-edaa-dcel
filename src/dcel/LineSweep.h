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

class LineSweep {
public:

    static void insertInStatusTree(
            std::vector<std::shared_ptr<HalfEdge<GeographicPoint>>>& statusTree,
            std::shared_ptr<HalfEdge<GeographicPoint>> edge
            );

    static void removeFromStatusTree(
            std::vector<std::shared_ptr<HalfEdge<GeographicPoint>>>& statusTree,
            std::shared_ptr<HalfEdge<GeographicPoint>> edge
    );

    static void swapWithNeighborInStatusTree(
            std::vector<std::shared_ptr<HalfEdge<GeographicPoint>>>& statusTree,
            std::shared_ptr<HalfEdge<GeographicPoint>> edge
    );


    static std::vector<Intersection<GeographicPoint>> findIntersections(
            const std::set<std::shared_ptr<HalfEdge<GeographicPoint>>>& edges);

    static void handleEventPoint(
            const Event& event,
            std::priority_queue<Event>& eventQ,
            std::vector<std::shared_ptr<HalfEdge<GeographicPoint>>>& statusTree
    );

    static void findNewEvent(
            const std::shared_ptr<HalfEdge<GeographicPoint>>& sl,
            const std::shared_ptr<HalfEdge<GeographicPoint>>& sr,
            const Event& p,
            std::priority_queue<Event>& eventQ,
    );
};


