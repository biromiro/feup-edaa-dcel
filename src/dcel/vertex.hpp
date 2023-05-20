// Copyright 2023 EDAA Group C
#pragma once

#include <memory>

#include "./half_edge.hpp"

template <class T>
class Vertex {
 public:
  explicit Vertex(T value) : incident(), value(value) {}

  Vertex(Vertex &&) noexcept = default;
  Vertex(const Vertex &) = default;
  Vertex &operator=(Vertex &&) = default;
  Vertex &operator=(const Vertex &) = default;

  [[nodiscard]] T getValue() const { return this->value; }

  void setIncident(const std::shared_ptr<HalfEdge<T>> &edge) {
    this->incident = edge;
  }

  bool operator<(const Vertex &rhs) const { return value < rhs.value; }

  std::shared_ptr<HalfEdge<T>> getIncident() { return this->incident; }

private:
  T value;
  std::shared_ptr<HalfEdge<T>> incident;
};
