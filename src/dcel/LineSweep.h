// Copyright (c) 2023 EDAA Group C. All Rights Reserved.
#pragma once

#include <iostream>
#include <queue>
#include <vector>

#include "./half_edge.hpp"
#include "Intersection.h"
#include "auxiliary.h"
#include "GeographicPoint.h"
#include "Event.h"
#include "Segment.h"
#include <algorithm>




class LineSweep {
public:

    static void insertInStatusTree(
            std::vector<std::shared_ptr<Segment>>& statusTree,
            GeographicPoint endpoint,
            const std::set<std::shared_ptr<Segment>, SegmentComparator>& segmentsToInsert
            );

    static void removeFromStatusTree(
            std::vector<std::shared_ptr<Segment>>& statusTree,
            GeographicPoint endpoint,
            const std::set<std::shared_ptr<Segment>, SegmentComparator>& segmentsToInsert
    );


    static int findLeftNeighborInStatusTree(
            std::vector<std::shared_ptr<Segment>>& statusTree,
            GeographicPoint endpoint,
            const std::shared_ptr<Segment>& segment
    );

    static int findRightNeighborInStatusTree(
            std::vector<std::shared_ptr<Segment>>& statusTree,
            GeographicPoint endpoint,
            const std::shared_ptr<Segment>& segment
    );


    static std::vector<std::shared_ptr<Intersection>> findIntersections(
            std::shared_ptr<DCEL<GeographicPoint>>& dcel);

    static void handleEventPoint(
            const std::shared_ptr<Event>& event,
            std::set<std::shared_ptr<Event>, EventComparator>& events,
            std::priority_queue<std::shared_ptr<Event>, std::vector<std::shared_ptr<Event>>, EventComparator>& eventQ,
            std::vector<std::shared_ptr<Intersection>>& intersections,
            std::vector<std::shared_ptr<Segment>>& statusTree,
            std::shared_ptr<DCEL<GeographicPoint>>& dcel
    );

    static void findNewEvent(
            const std::shared_ptr<Segment>& sl,
            const std::shared_ptr<Segment>& sr,
            const std::shared_ptr<Event>& p,
            std::set<std::shared_ptr<Event>, EventComparator>& events,
            std::priority_queue<std::shared_ptr<Event>, std::vector<std::shared_ptr<Event>>, EventComparator>& eventQ
    );
};


