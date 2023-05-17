// Copyright (c) 2023 EDAA Group C. All Rights Reserved.

#include "./LineSweep.h"

#include <vector>

#include "GeographicPoint.h"

/**
 * I need to first unite the half edges into line segments, but I need to do so
 * without increasing the space complexity
 */

std::vector<Intersection<GeographicPoint>> LineSweep::findIntersections(
        const std::set<std::shared_ptr<HalfEdge<GeographicPoint>>>& edges
        ) {
    std::vector<Intersection<GeographicPoint>> intersections;

    // TODO(): Event Queue must have upper endpoints and lower endpoints
    //     for both input sets of endpoints

    std::priority_queue<Event> eventQ{};
    std::set<Event> events;
    std::vector<HalfEdge<GeographicPoint>> statusTree;

    for (const auto& segment : edges) {
        auto newEventUpper = Event(UPPER_ENDPOINT, segment);
        auto newEventLower = Event(LOWER_ENDPOINT, segment);

        auto previousEventUpperItr = events.find(newEventUpper),
             previousEventLowerItr = events.find(newEventLower);

        if ( previousEventUpperItr == events.end()) {
            events.insert(newEventUpper);
        } else {
            auto event = *previousEventUpperItr;
            events.erase(event);
            event.addEdge(segment);
            events.insert(event);
        }

        if ( previousEventLowerItr == events.end()) {
            events.insert(newEventLower);
        } else {
            auto event = *previousEventLowerItr;
            events.erase(event);
            event.addEdge(segment);
            events.insert(event);
        }
    }

    // TODO(): Figure out a BST to store the edges - better to use a vector and implement BST-like functions
    //  for lookup, insertion and swap

    while (!eventQ.empty()) {
        handleEventPoint(eventQ.top(), eventQ, statusTree);
        eventQ.pop();
    }

    return intersections;
}

void LineSweep::handleEventPoint(const Event &event, const std::priority_queue<Event> &eventQ,
                                 const std::vector<HalfEdge<GeographicPoint>> &statusTree) {


}
