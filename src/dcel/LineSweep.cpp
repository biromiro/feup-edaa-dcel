// Copyright (c) 2023 EDAA Group C. All Rights Reserved.

#include "./LineSweep.h"

#include <vector>

#include "GeographicPoint.h"

// TODO(): This logic should follow the endpoint of an edge - might be better to use an alternative structure
//          although it would be a lot more efficient if we stood with half edge pointers

struct StatusComparator {
    StatusComparator(double currentSweepLine) { this->currentSweepLine = currentSweepLine - EPSILON; }

    bool operator () (const std::shared_ptr<Segment>& s1, const std::shared_ptr<Segment>& s2) {
        auto segment1Long = s1->getSegmentCurrentLongitude(currentSweepLine),
                segment2Long = s2->getSegmentCurrentLongitude(currentSweepLine);

        if (s1->getUpperEndpoint() == s2->getUpperEndpoint()) {
            if (s1->getSlope() == 0) return false;
            if (s2->getSlope() == 0) return true;
        }

        return segment1Long < segment2Long;
    };

    double currentSweepLine;
};

/**
 * @param v - sorted vector instance
 * @param data - value to search
 * @return 0-based index if data found, -1 otherwise
*/
size_t binary_search_find_index(const std::vector<std::shared_ptr<Segment>>& statusTree,
                                const GeographicPoint& endpoint,
                                const std::shared_ptr<Segment> &segment) {
    auto currentSweepLine = endpoint.getLatitude();

    auto it = std::lower_bound(statusTree.begin(),
                               statusTree.end(),
                               segment, StatusComparator(currentSweepLine));
    if (it == statusTree.end() || *it != segment) {
        return -1;
    } else {
        std::size_t index = std::distance(statusTree.begin(), it);
        return index;
    }
}

void LineSweep::insertInStatusTree(std::vector<std::shared_ptr<Segment>> &statusTree,
                                   GeographicPoint endpoint,
                                   const std::set<std::shared_ptr<Segment>, SegmentComparator>& segmentsToInsert) {

    if (segmentsToInsert.empty())
        return;

    // the new order should follow what's immediately below the current sweep line
    auto currentSweepLine = endpoint.getLatitude();

    for (const auto& segment: segmentsToInsert) {
        auto itr = std::lower_bound(statusTree.begin(),
                                    statusTree.end(),
                                    segment, StatusComparator(currentSweepLine));
        statusTree.insert(itr, segment);
    }
}

void LineSweep::removeFromStatusTree(std::vector<std::shared_ptr<Segment>> &statusTree,
                                     GeographicPoint endpoint,
                                     const std::set<std::shared_ptr<Segment>, SegmentComparator>& segmentsToRemove) {

    for (const auto& segment: segmentsToRemove) {
        auto it = std::find_if(statusTree.begin(),
                               statusTree.end(), [segment](const std::shared_ptr<Segment>& s1) {
            return (s1->getVertex1() == segment->getVertex1() && s1->getVertex2() == segment->getVertex2()) ||
                   (s1->getVertex1() == segment->getVertex2() && s1->getVertex2() == segment->getVertex1());
        });

        if (it != statusTree.end())
            statusTree.erase(it);
    }


}

int LineSweep::findLeftNeighborInStatusTree(std::vector<std::shared_ptr<Segment>> &statusTree,
                                            GeographicPoint endpoint,
                                            const std::shared_ptr<Segment> &segment) {

    auto idx = binary_search_find_index(statusTree, endpoint, segment);
    if (idx == -1) return -1;
    return idx - 1;

}

int LineSweep::findRightNeighborInStatusTree(std::vector<std::shared_ptr<Segment>> &statusTree, GeographicPoint endpoint, const std::shared_ptr<Segment> &segment) {
    auto idx = binary_search_find_index(statusTree, endpoint, segment);
    if (idx == -1 || idx + 1== statusTree.size()) return -1;
    return idx + 1;
}


std::vector<std::shared_ptr<Intersection>> LineSweep::findIntersections(
        std::shared_ptr<DCEL<GeographicPoint>>& dcel
        ) {
    std::vector<std::shared_ptr<Intersection>> intersections;

    std::priority_queue<std::shared_ptr<Event>, std::vector<std::shared_ptr<Event>>, EventComparator> eventQ;
    std::set<std::shared_ptr<Event>, EventComparator> events;
    std::vector<std::shared_ptr<Segment>> statusTree;

    for (const auto& segment : dcel->getEdges()) {
        auto newEvent = std::make_shared<Event>(segment->getOrigin()->getValue());

        auto previousEventItr = events.find(newEvent);
        auto newSegment = std::make_shared<Segment>(segment);
        if ( previousEventItr == events.end()) {
            newEvent->addSegment(newSegment);
            events.insert(newEvent);
        } else {
            auto& event = *previousEventItr;
            event->addSegment(newSegment);
        }
    }

    for (const auto& event: events) {
        eventQ.push(event);
    }

    while (!eventQ.empty()) {
        handleEventPoint(eventQ.top(), events, eventQ, intersections, statusTree);
        eventQ.pop();
    }

    return intersections;
}

void LineSweep::handleEventPoint(const std::shared_ptr<Event> &event,
                                 std::set<std::shared_ptr<Event>, EventComparator>& events,
                                 std::priority_queue<std::shared_ptr<Event>, std::vector<std::shared_ptr<Event>>, EventComparator> &eventQ,
                                 std::vector<std::shared_ptr<Intersection>>& intersections,
                                 std::vector<std::shared_ptr<Segment>> &statusTree) {

    auto endpoint = event->getEndpoint();
    const std::set<std::shared_ptr<Segment>, SegmentComparator>& upperSegments = event->getSegments();
    auto lowerSegments = std::set<std::shared_ptr<Segment>, SegmentComparator>(), intersectingSegments = std::set<std::shared_ptr<Segment>, SegmentComparator>();

    for (const auto& segment: statusTree) {
        if (segment->getLowerEndpoint() == endpoint)
            lowerSegments.insert(segment);
        else if (approximatelyEqual(endpoint.getLongitude(), segment->getSegmentCurrentLongitude(endpoint.getLatitude())))
            intersectingSegments.insert(segment);
        else if (segment->getSlope() == 0 && (
                endpoint.getLongitude() >= segment->getUpperEndpoint().getLongitude() &&
                endpoint.getLongitude() <= segment->getLowerEndpoint().getLongitude()
                ))
            intersectingSegments.insert(segment);
    }

    if (upperSegments.size() + lowerSegments.size() + intersectingSegments.size() > 1){
        auto intersection = std::make_shared<Intersection>(endpoint);
        for (const auto& segment: upperSegments)
            intersection->addEdge(segment->getEdge());
        for (const auto& segment: lowerSegments)
            intersection->addEdge(segment->getEdge());
        for (const auto& segment: intersectingSegments)
            intersection->addEdge(segment->getEdge());
        intersections.push_back(intersection);
    }

    removeFromStatusTree(statusTree, endpoint, lowerSegments);
    removeFromStatusTree(statusTree, endpoint, intersectingSegments);

    insertInStatusTree(statusTree, endpoint, upperSegments);
    insertInStatusTree(statusTree, endpoint, intersectingSegments);


    if (upperSegments.size() + intersectingSegments.size() == 0) {

        for (auto itr = statusTree.begin(); itr != statusTree.end(); itr++) {
            if ((*itr)->getSegmentCurrentLongitude(endpoint.getLatitude()) > endpoint.getLongitude()) {
                if (itr != statusTree.begin())
                    findNewEvent(*(itr - 1), *itr, event, events, eventQ);
            }
        }

    } else {
        std::vector<std::shared_ptr<Segment>> upperAndIntersectingSegments;
        auto currentSweepLine = endpoint.getLatitude();
        std::copy(upperSegments.begin(),
                  upperSegments.end(),
                  std::back_inserter(upperAndIntersectingSegments));

        std::copy(intersectingSegments.begin(),
                  intersectingSegments.end(),
                  std::back_inserter(upperAndIntersectingSegments));

        std::sort(upperAndIntersectingSegments.begin(),
                  upperAndIntersectingSegments.end(), StatusComparator(currentSweepLine));

        auto sl = findLeftNeighborInStatusTree(statusTree, endpoint, upperAndIntersectingSegments.front());

        if (sl != -1)
            findNewEvent(statusTree.at(sl), upperAndIntersectingSegments.front(), event, events, eventQ);

        auto sr = findRightNeighborInStatusTree(statusTree, endpoint, upperAndIntersectingSegments.back());

        if (sr != -1)
            findNewEvent(statusTree.at(sr), upperAndIntersectingSegments.back(), event, events, eventQ);

    }

}

void LineSweep::findNewEvent(const std::shared_ptr<Segment> &sl,
                             const std::shared_ptr<Segment> &sr, const std::shared_ptr<Event> &p,
                             std::set<std::shared_ptr<Event>, EventComparator>& events,
                             std::priority_queue<std::shared_ptr<Event>, std::vector<std::shared_ptr<Event>>, EventComparator> &eventQ) {

    if (!edgesIntersect(sl->getEdge(), sr->getEdge()))
        return;

    auto intersectionPoint = calculateIntersectionPoint(sl->getEdge(), sr->getEdge());

    if (intersectionPoint.getLatitude() < p->getEndpoint().getLatitude() ||
            (approximatelyEqual(p->getEndpoint().getLatitude(),intersectionPoint.getLatitude())
            && p->getEndpoint().getLongitude() < intersectionPoint.getLongitude())
            ) {

        auto newEvent = std::make_shared<Event>(intersectionPoint);
        auto previousEventItr = events.find(newEvent);

        if ( previousEventItr == events.end()) {
            newEvent->addSegment(sl);
            newEvent->addSegment(sr);
            eventQ.push(newEvent);
            events.insert(newEvent);
        } else {
            (*previousEventItr)->addSegment(sl);
            (*previousEventItr)->addSegment(sr);
        }
    }
}



