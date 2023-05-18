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
#include "Segment.h"

// TODO: Should store the endpoint and have a function that returns a ordered value or overload operator
//       to order events


class Event {
public:
    explicit Event(const GeographicPoint& endpoint);
    [[nodiscard]] const GeographicPoint &getEndpoint() const;

    void addSegment(const std::shared_ptr<Segment>& segment);

    [[nodiscard]] const std::set<std::shared_ptr<Segment>, SegmentComparator> &getSegments() const;

    friend std::ostream &operator<<(std::ostream &os, const Event &event);

private:
    std::set<std::shared_ptr<Segment>, SegmentComparator> segments;
    GeographicPoint endpoint;
};

struct EventComparator {
    bool operator()(const std::shared_ptr<Event>& left, const std::shared_ptr<Event>& right) const {
        return left->getEndpoint() < right->getEndpoint();
    };
};

#endif //DCEL_EVENT_H
