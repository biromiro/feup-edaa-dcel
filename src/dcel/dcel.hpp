// Copyright 2023 EDAA Group C
#pragma once

#include "face.hpp"
#include "vertex.hpp"

#include <memory>
#include <vector>
#include <set>

template <class T>
class DCEL {
public:

    DCEL() {
        this->vertices = std::set<std::shared_ptr<Vertex<T>>>();
        this->edges = std::set<std::shared_ptr<HalfEdge<T>>>();
        this->faces = std::set<std::shared_ptr<Face<T>>>();
    };

    DCEL(DCEL<T> &&)  noexcept = default;
    DCEL(const DCEL<T> &) = default;
    DCEL &operator=(DCEL<T> &&) = default;
    DCEL &operator=(const DCEL<T> &) = default;

    std::set<std::shared_ptr<Face<T>>> getFaces() { return this->edges; };
    std::set<std::shared_ptr<HalfEdge<T>>> getEdges() { return this->faces; };
    std::set<std::shared_ptr<Vertex<T>>> getVertices() { return this->vertices; };

    void addFace(std::shared_ptr<Face<T>> face) {
        this->faces.insert(face);
    }

    void addEdge(std::shared_ptr<HalfEdge<T>> edge) {
        this->edges.insert(edge);
    }

    void addVertex(std::shared_ptr<Vertex<T>> vertex) {
        this->vertices.insert(vertex);
    }

private:
    std::set<std::shared_ptr<Face<T>>> faces;
    std::set<std::shared_ptr<HalfEdge<T>>> edges;
    std::set<std::shared_ptr<Vertex<T>>> vertices;
};
