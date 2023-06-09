//
// Created by nrtc on 5/18/2023.
//

#ifndef DCEL_SEGMENT_H
#define DCEL_SEGMENT_H

#include <iostream>
#include "../../../dcel_substructures/HalfEdge.tpp"
#include "../../../dcel_substructures/Vertex.tpp"
#include "../../../auxiliary/Auxiliary.h"
#include "../../../auxiliary/geographic_point/GeographicPoint.h"

class Segment{
public:

    explicit Segment(const std::shared_ptr<HalfEdge<GeographicPoint>>& edge);
    [[nodiscard]] GeographicPoint getUpperEndpoint() const;

    [[nodiscard]] GeographicPoint getLowerEndpoint() const;

    [[nodiscard]] const std::shared_ptr<HalfEdge<GeographicPoint>> &getEdge() const;
    [[nodiscard]] double getSegmentCurrentLongitude(double latitude) const;

    bool operator<(const Segment &rhs) const;

    bool operator==(const Segment &rhs) const;

    bool operator!=(const Segment &rhs) const;

    [[nodiscard]] const GeographicPoint &getVertex1() const;

    [[nodiscard]] const GeographicPoint &getVertex2() const;

    friend std::ostream &operator<<(std::ostream &os, const Segment &segment);

    double getSlope() const;

private:
    std::shared_ptr<HalfEdge<GeographicPoint>> edge;
    GeographicPoint vertex1;
    GeographicPoint vertex2;
    double slope;
    double bias;

};

struct SegmentComparator {
    bool operator()(const std::shared_ptr<Segment>& left, const std::shared_ptr<Segment>& right) const {
        if (left->getUpperEndpoint() == right->getUpperEndpoint())
            return left->getLowerEndpoint() < right->getLowerEndpoint();
        return left->getUpperEndpoint() < right->getUpperEndpoint();
    };
};

#endif //DCEL_SEGMENT_H
