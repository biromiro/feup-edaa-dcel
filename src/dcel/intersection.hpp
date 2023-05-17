// Copyright 2023 EDAA Group C
#pragma once

#include <memory>

#include "vertex.hpp"
#include "half_edge.hpp"

template <class T>
class Intersection {
public:
    Intersection(std::shared_ptr<Vertex<T>> intersectionPoint,
                 std::shared_ptr<HalfEdge<T>> edge1,
                 std::shared_ptr<HalfEdge<T>> edge2) :
                 intersectionPoint(intersectionPoint),
                 edge1(edge1), edge2(edge2) {}

    const std::shared_ptr<Vertex<T>> &getIntersectionPoint() const {
        return intersectionPoint;
    }

    const std::shared_ptr<HalfEdge<T>> &getEdge1() const {
        return edge1;
    }

    const std::shared_ptr<HalfEdge<T>> &getEdge2() const {
        return edge2;
    }

private:
    std::shared_ptr<Vertex<T>> intersectionPoint;
    std::shared_ptr<HalfEdge<T>> edge1;
    std::shared_ptr<HalfEdge<T>> edge2;
};
