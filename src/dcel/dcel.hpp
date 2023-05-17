// Copyright 2023 EDAA Group C
#pragma once

#include <memory>
#include <set>
#include <vector>

#include "./face.hpp"
#include "./vertex.hpp"
#include "GeographicPoint.h"
template <class T>
struct VertexComparator {
  bool operator()(const std::shared_ptr<Vertex<T>> lhs,
                  const std::shared_ptr<Vertex<T>> rhs) const {
    return *lhs < *rhs;
  }
};

template <class T>
struct FaceComparator {
  bool operator()(const std::shared_ptr<Face<T>> lhs,
                  const std::shared_ptr<Face<T>> rhs) const {
    return *lhs < *rhs;
  }
};

template <class T>
class DCEL {
    public:
        DCEL() {
            this->vertices = std::set<std::shared_ptr<Vertex<T>>, VertexComparator<T>>();
            this->edges = std::set<std::shared_ptr<HalfEdge<T>>>();
            this->faces = std::set<std::shared_ptr<Face<T>>, FaceComparator<T>>();
            this->unboundedFace = std::make_shared<Face<T>>();
            this->faces.insert(this->unboundedFace);
        };

        DCEL(DCEL<T> &&)  noexcept = default;
        DCEL(const DCEL<T> &) = default;
        DCEL &operator=(DCEL<T> &&) = default;
        DCEL &operator=(const DCEL<T> &) = default;

          std::set<std::shared_ptr<Face<T>>, FaceComparator<T>> getFaces() {
            return this->faces;
          }
          std::set<std::shared_ptr<HalfEdge<T>>> getEdges() { return this->edges; }
          std::set<std::shared_ptr<Vertex<T>>, VertexComparator<T>> getVertices() {
            return this->vertices;
          }

          void addFace(std::shared_ptr<Face<T>> face) { this->faces.insert(face); }

          void addEdge(std::shared_ptr<HalfEdge<T>> edge) { this->edges.insert(edge); }

          void addVertex(std::shared_ptr<Vertex<T>> vertex) {
            this->vertices.insert(vertex);
          }

          std::shared_ptr<Vertex<T>> getVertex(const T& val) {
            auto value = vertices.find(std::make_shared<Vertex<T>>(val));

            return value == vertices.end() ? std::shared_ptr<Vertex<T>>(nullptr)
                                           : (*value);
          }

          std::shared_ptr<HalfEdge<T>> getHalfEdge(
              const std::shared_ptr<Vertex<T>>& orig,
              const std::shared_ptr<Vertex<T>>& dest) {
            auto edge = std::find_if(
                edges.begin(), edges.end(),
                [&orig, &dest](const std::shared_ptr<HalfEdge<T>>& hedge) {
                  const std::shared_ptr<Vertex<T>>& originVertex = hedge->getOrigin();
                  const std::shared_ptr<Vertex<T>>& destVertex =
                      (hedge->getTwin())->getOrigin();
                  return (originVertex.get() == orig.get()) &&
                         (destVertex.get() == dest.get());
                });

            return edge == edges.end() ? std::shared_ptr<HalfEdge<T>>(nullptr)
                                       : (*edge);
          }

          std::shared_ptr<Face<T>> getFace(const nlohmann::json& property) {
            auto value = faces.find(std::make_shared<Face<T>>(property));

            return value == vertices.end() ? std::shared_ptr<Face<T>>(nullptr)
                                           : (*value);
          }

        const std::shared_ptr<Face<T>> &getUnboundedFace() const {
            return unboundedFace;
        }

    private:
        std::set<std::shared_ptr<Face<T>>, FaceComparator<T>> faces;
        std::set<std::shared_ptr<HalfEdge<T>>> edges;
        std::set<std::shared_ptr<Vertex<T>>, VertexComparator<T>> vertices;
        std::shared_ptr<Face<T>> unboundedFace;
};
