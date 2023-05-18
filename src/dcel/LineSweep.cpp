// Copyright (c) 2023 EDAA Group C. All Rights Reserved.

#include "./LineSweep.h"

#include <vector>

#include "GeographicPoint.h"

Segment::Segment(const std::shared_ptr<HalfEdge<GeographicPoint>> &edge) {
    this->vertex1 = edge->getOrigin()->getValue();
    this->vertex2 = edge->getTwin()->getOrigin()->getValue();
    this->slope = (vertex2.getLatitude() - vertex1.getLatitude()) / (vertex2.getLongitude() - vertex1.getLongitude());
    this->bias = vertex1.getLatitude() - this->slope * vertex1.getLongitude();
}

GeographicPoint Segment::getUpperEndpoint() const {
    return vertex1 < vertex2 ? vertex1 : vertex2;
}

GeographicPoint Segment::getLowerEndpoint() const {
    return vertex1 < vertex2 ? vertex2 : vertex1;
}

double Segment::getSegmentCurrentLongitude(double latitude) const {
    return (latitude - this->bias) / this->slope;
}

const std::shared_ptr<HalfEdge<GeographicPoint>> &Segment::getEdge() const {
    return edge;
}

// TODO(): This logic should follow the endpoint of an edge - might be better to use an alternative structure
//          although it would be a lot more efficient if we stood with half edge pointers

void LineSweep::insertInStatusTree(std::vector<Segment> &statusTree,
                                   const Event& event) {
    // TODO: Create Segment and check where to add it


}

void LineSweep::removeFromStatusTree(std::vector<Segment> &statusTree,
                                     const Event& event) {



}

void LineSweep::swapWithNeighborInStatusTree(std::vector<Segment> &statusTree,
                                             const Event& event) {


}

std::vector<Intersection<GeographicPoint>> LineSweep::findIntersections(
        const std::set<std::shared_ptr<HalfEdge<GeographicPoint>>>& edges
        ) {
    std::vector<Intersection<GeographicPoint>> intersections;

    // TODO(): Event Queue must have upper endpoints and lower endpoints
    //     for both input sets of endpoints

    std::priority_queue<Event> eventQ{};
    std::set<Event> events;
    std::vector<Segment> statusTree;

    for (const auto& segment : edges) {
        auto newEventUpper = Event(segment->getOrigin()->getValue());
        auto newEventLower = Event(segment->getTwin()->getOrigin()->getValue());

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
                                 std::vector<Segment> &statusTree) {


}

void LineSweep::findNewEvent(const std::shared_ptr<HalfEdge<GeographicPoint>> &sl,
                             const std::shared_ptr<HalfEdge<GeographicPoint>> &sr, const Event &p,
                             std::priority_queue<Event> &eventQ) {

    auto intersectionPoint = calculateIntersectionPoint(sl, sr);
    if (p.getEndpoint().getLongitude() < intersectionPoint.getLongitude() ||
            (p.getEndpoint().getLongitude() == intersectionPoint.getLongitude()
            && p.getEndpoint().getLatitude() < intersectionPoint.getLatitude())
            )
        eventQ.push(Event(intersectionPoint));
    // TODO: Add edges to intersection point event
}

