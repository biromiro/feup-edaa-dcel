//
// Created by nrtc on 5/17/2023.
//

#include "Event.h"

Event::Event(const GeographicPoint& endpoint) {

    this->segments = std::set<std::shared_ptr<Segment>, SegmentComparator>();
    this->endpoint = endpoint;
}

void Event::addSegment(const std::shared_ptr<Segment> &segment) {
    if (segment->getUpperEndpoint() == this->endpoint)
        (this->segments).insert(segment);
}

const GeographicPoint &Event::getEndpoint() const {
    return endpoint;
}

const std::set<std::shared_ptr<Segment>, SegmentComparator> &Event::getSegments() const {
    return segments;
}

std::ostream &operator<<(std::ostream &os, const Event &event) {
    os << "number of segments: " << event.segments.size() << " endpoint: " << event.endpoint << "\n";
    os << "segments:\n";
    for (auto segment: event.segments)
        os << segment << "\n";
    return os;
}


