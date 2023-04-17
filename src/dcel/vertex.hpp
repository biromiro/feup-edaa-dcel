// Copyright 2023 EDAA Group C
#pragma once

#include <memory>

#include "./half_edge.hpp"

template <class T>
class Vertex {
 public:
  explicit Vertex(T value) : incident() { this->value = value; }

  Vertex(Vertex &&) = default;
  Vertex(const Vertex &) = default;
  Vertex &operator=(Vertex &&) = default;
  Vertex &operator=(const Vertex &) = default;
  ~Vertex() { delete this->incident; }

  [[nodiscard]] T getValue() const { return this->value; }
  HalfEdge<T> *getIncident() { return this->incident; }

 private:
  T value;
  HalfEdge<T> *incident;
};
