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
    static std::vector<Intersection<GeographicPoint>> findIntersections(
            const std::set<std::shared_ptr<HalfEdge<GeographicPoint>>>& edges);

    static void handleEventPoint(
            const Event& event,
            const std::priority_queue<Event>& eventQ,
            const std::vector<HalfEdge<GeographicPoint>>& statusTree
    );
};


