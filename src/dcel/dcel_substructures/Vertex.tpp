// Copyright 2023 EDAA Group C
#pragma once

#include <memory>

#include "./HalfEdge.tpp"

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

    const std::pair<std::shared_ptr<HalfEdge<T>>, std::shared_ptr<HalfEdge<T>>> &getLeftEdge() const {
        return leftEdge;
    }

    void setLeftEdge(const std::pair<std::shared_ptr<HalfEdge<T>>, std::shared_ptr<HalfEdge<T>>> &leftEdge) {
        Vertex::leftEdge = leftEdge;
    }

private:
  T value;
  std::shared_ptr<HalfEdge<T>> incident;
  std::pair<std::shared_ptr<HalfEdge<T>>, std::shared_ptr<HalfEdge<T>>> leftEdge;
};
