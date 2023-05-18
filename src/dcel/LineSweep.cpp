// Copyright (c) 2023 EDAA Group C. All Rights Reserved.

#include "./LineSweep.h"

#include <vector>

#include "GeographicPoint.h"

/**
 * I need to first unite the half edges into line segments, but I need to do so
 * without increasing the space complexity
 */

// TODO(): This logic should follow the endpoint of an edge - might be better to use an alternative structure
//          although it would be a lot more efficient if we stood with half edge pointers

void LineSweep::insertInStatusTree(std::vector<std::shared_ptr<HalfEdge<GeographicPoint>>> &statusTree,
                                   std::shared_ptr<HalfEdge<GeographicPoint>> edge) {
    auto itr = std::lower_bound(statusTree.begin(), statusTree.end(), edge);
}

void LineSweep::removeFromStatusTree(std::vector<std::shared_ptr<HalfEdge<GeographicPoint>>> &statusTree,
                                     std::shared_ptr<HalfEdge<GeographicPoint>> edge) {

}

void LineSweep::swapWithNeighborInStatusTree(std::vector<std::shared_ptr<HalfEdge<GeographicPoint>>> &statusTree,
                                             std::shared_ptr<HalfEdge<GeographicPoint>> edge) {

}

std::vector<Intersection<GeographicPoint>> LineSweep::findIntersections(
        const std::set<std::shared_ptr<HalfEdge<GeographicPoint>>>& edges
        ) {
    std::vector<Intersection<GeographicPoint>> intersections;

    // TODO(): Event Queue must have upper endpoints and lower endpoints
    //     for both input sets of endpoints

    std::priority_queue<Event> eventQ{};
    std::set<Event> events;
    std::vector<std::shared_ptr<HalfEdge<GeographicPoint>>> statusTree;

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

void LineSweep::handleEventPoint(const Event &event, std::priority_queue<Event> &eventQ,
                                 std::vector<std::shared_ptr<HalfEdge<GeographicPoint>>> &statusTree) {


}

void LineSweep::findNewEvent(const std::shared_ptr<HalfEdge<GeographicPoint>> &sl,
                             const std::shared_ptr<HalfEdge<GeographicPoint>> &sr, const Event &p,
                             std::priority_queue<Event> &eventQ) {

    auto intersectionPoint = calculateIntersectionPoint(sl, sr);
    if (p.getEndpoint().getLongitude() < intersectionPoint.getLongitude() ||
            (p.getEndpoint().getLongitude() == intersectionPoint.getLongitude()
            && p.getEndpoint().getLatitude() < intersectionPoint.getLatitude())
            )
        eventQ.push(Event(INTERSECTION, sl, sr));
}
