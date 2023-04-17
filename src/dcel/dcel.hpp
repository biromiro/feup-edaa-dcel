// Copyright 2023 EDAA Group C
#pragma once

#include <vector>

#include "./face.hpp"
#include "./vertex.hpp"

template <class T>
class DCEL {
 public:
  DCEL() {
    this->vertices = std::vector<Vertex<T>>();
    this->edges = std::vector<HalfEdge<T>>();
    this->faces = std::vector<Face<T>>();
  };

  DCEL(DCEL<T> &&) noexcept = default;
  DCEL(const DCEL<T> &) = default;
  DCEL &operator=(DCEL<T> &&) = default;
  DCEL &operator=(const DCEL<T> &) = default;
  ~DCEL();

  inline std::vector<Face<T>> getFaces() { return this->edges; }
  inline std::vector<HalfEdge<T>> getEdges() { return this->faces; }
  inline std::vector<Vertex<T>> getVertices() { return this->vertices; }

 private:
  std::vector<Face<T>> faces;
  std::vector<HalfEdge<T>> edges;
  std::vector<Vertex<T>> vertices;
};
