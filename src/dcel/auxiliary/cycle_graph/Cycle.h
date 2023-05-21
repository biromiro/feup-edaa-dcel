//
// Created by nrtc on 5/21/2023.
//

#ifndef DCEL_CYCLE_H
#define DCEL_CYCLE_H


#include <vector>
#include <memory>
#include "../../dcel_substructures/HalfEdge.tpp"
#include "../geographic_point/GeographicPoint.h"


class Cycle {
public:
    Cycle(bool isUnbounded = false);
    void addEdgeToCycle(const std::shared_ptr<HalfEdge<GeographicPoint>>& edge);
    [[nodiscard]] std::shared_ptr<Vertex<GeographicPoint>> getLeftmostVertex() const;
    [[nodiscard]] const std::vector<std::shared_ptr<HalfEdge<GeographicPoint>>> &getCycle() const;
    [[nodiscard]] double isBoundaryCycle() const;
    void reverseCycle();
    bool isUnboundedCycle() const;
    size_t getID() const;
    void setID(size_t id);
private:
    size_t id;
    std::vector<std::shared_ptr<HalfEdge<GeographicPoint>>> cycleVec;
    bool unboundedCycle;
};


#endif //DCEL_CYCLE_H
