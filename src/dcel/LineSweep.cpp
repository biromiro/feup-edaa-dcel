// Copyright (c) 2023 EDAA Group C. All Rights Reserved.

#include "./LineSweep.h"

#include <vector>

#include "GeographicPoint.h"

// TODO(): This logic should follow the endpoint of an edge - might be better to use an alternative structure
//          although it would be a lot more efficient if we stood with half edge pointers

void LineSweep::insertInStatusTree(std::vector<std::shared_ptr<Segment>> &statusTree,
                                   GeographicPoint endpoint,
                                   const std::set<std::shared_ptr<Segment>, SegmentComparator>& segmentsToInsert) {

    if (segmentsToInsert.empty())
        return;

    // the new order should follow what's immediately below the current sweep line
    auto currentSweepLine = endpoint.getLatitude() + 0.01;

    for (const auto& segment: segmentsToInsert) {
        auto itr = std::lower_bound(statusTree.begin(),
                                    statusTree.end(),
                                    segment, [currentSweepLine](const std::shared_ptr<Segment>& s1, const std::shared_ptr<Segment>& s2) {
            auto segment1Lat = s1->getSegmentCurrentLongitude(currentSweepLine),
                    segment2Lat = s2->getSegmentCurrentLongitude(currentSweepLine);
            return segment1Lat < segment2Lat;
        });
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

    auto currentSweepLine = endpoint.getLatitude();
    auto itr = std::lower_bound(statusTree.begin(), statusTree.end(), segment, [currentSweepLine](const std::shared_ptr<Segment>& s1, const std::shared_ptr<Segment>& s2) {
        auto segment1Lat = s1->getSegmentCurrentLongitude(currentSweepLine),
                segment2Lat = s2->getSegmentCurrentLongitude(currentSweepLine);
        return segment1Lat < segment2Lat;
    });

    int distance = std::distance(statusTree.begin(), itr);
    if (distance >= statusTree.size()) {
        distance = -1;
    } else if ((*itr) == segment) {
        distance -= 1;
    }
    return distance;
}

int LineSweep::findRightNeighborInStatusTree(std::vector<std::shared_ptr<Segment>> &statusTree, GeographicPoint endpoint, const std::shared_ptr<Segment> &segment) {
    auto currentSweepLine = endpoint.getLatitude();
    auto itr = std::upper_bound(statusTree.begin(), statusTree.end(), segment, [currentSweepLine](const std::shared_ptr<Segment>& s1, const std::shared_ptr<Segment>& s2) {
        auto segment1Lat = s1->getSegmentCurrentLongitude(currentSweepLine),
                segment2Lat = s2->getSegmentCurrentLongitude(currentSweepLine);
        return segment1Lat < segment2Lat;
    });

    int distance = std::distance(statusTree.begin(), itr);
    if (distance >= statusTree.size())
        distance = -1;
    return distance;
}


std::vector<std::shared_ptr<Intersection>> LineSweep::findIntersections(
        const std::set<std::shared_ptr<HalfEdge<GeographicPoint>>>& edges
        ) {
    std::vector<std::shared_ptr<Intersection>> intersections;

    std::priority_queue<std::shared_ptr<Event>, std::vector<std::shared_ptr<Event>>, EventComparator> eventQ;
    std::set<std::shared_ptr<Event>, EventComparator> events;
    std::vector<std::shared_ptr<Segment>> statusTree;

    std::cout << "Generating eventq for " << edges.size() << " edges" <<
    std::flush << std::endl;

    for (const auto& segment : edges) {
        auto newEvent = std::make_shared<Event>(segment->getOrigin()->getValue());

        auto previousEventItr = events.find(newEvent);
        auto newSegment = std::make_shared<Segment>(segment);
        if ( previousEventItr == events.end()) {
            newEvent->addSegment(newSegment);
            events.insert(newEvent);
        } else {
            const auto& event = *previousEventItr;
            event->addSegment(newSegment);
        }
    }

    for (const auto& event: events) {
        eventQ.push(event);
    }

    std::cout << "Generated eventq with " << eventQ.size() << " entries" << std::flush << std::endl;

    while (!eventQ.empty()) {
        //std::cout << "Handling event " << *(eventQ.top()) << std::endl;
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
        else if (endpoint.getLongitude() == segment->getSegmentCurrentLongitude(endpoint.getLatitude()))
            intersectingSegments.insert(segment);
    }

    if (upperSegments.size() + lowerSegments.size() + intersectingSegments.size() > 1){
        std::cout << "Found an intersection" << std::endl;
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
                  upperAndIntersectingSegments.begin(), [currentSweepLine](const std::shared_ptr<Segment>& s1, const std::shared_ptr<Segment>& s2) {
            return s1->getSegmentCurrentLongitude(currentSweepLine) < s2->getSegmentCurrentLongitude(currentSweepLine);
        });

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

    if (p->getEndpoint().getLongitude() < intersectionPoint.getLongitude() ||
            (p->getEndpoint().getLongitude() == intersectionPoint.getLongitude()
            && p->getEndpoint().getLatitude() < intersectionPoint.getLatitude())
            ) {

        std::cout << "Intersection between  " << sl << " and " << sr << " at " << intersectionPoint << std::flush << std::endl;
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



