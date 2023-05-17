// Copyright 2023 EDAA Group C
#pragma once

#include <memory>

template <class T>
class Vertex;

template <class T>
class Face;

template <class T>
class HalfEdge {
public:
    HalfEdge(): incident(), next(), prev(), twin(), origin() {};
    explicit HalfEdge(const std::shared_ptr<Vertex<T>>& origin): incident(), next(), prev(), twin(), origin(origin) {};
    HalfEdge(const std::shared_ptr<Vertex<T>>& origin, const std::shared_ptr<Face<T>>& face): incident(face), next(), prev(), twin(), origin(origin) {};
    HalfEdge(HalfEdge &&) = default;
    HalfEdge(const HalfEdge &) = default;
    HalfEdge &operator=(HalfEdge &&) = default;
    HalfEdge &operator=(const HalfEdge &) = default;

  void setOrigin(const std::shared_ptr<Vertex<T>> &origin_) {
    this->origin = origin_;
  }

  void setTwin(const std::shared_ptr<HalfEdge<T>> &twin_) {
    this->twin = twin_;
  }

  void setNext(const std::shared_ptr<HalfEdge<T>> &next_) {
    this->next = next_;
  }

  void setPrev(const std::shared_ptr<HalfEdge<T>> &prev_) {
    this->prev = prev_;
  }

  void setIncident(const std::shared_ptr<Face<T>> &incident_) {
    this->incident = incident_;
  }

  const std::shared_ptr<Vertex<T>> &getOrigin() const { return origin; }

  const std::shared_ptr<HalfEdge<T>> &getTwin() const { return twin; }

  const std::shared_ptr<HalfEdge<T>> &getNext() const { return next; }

  const std::shared_ptr<HalfEdge<T>> &getPrev() const { return prev; }

  const std::shared_ptr<Face<T>> &getIncident() const { return incident; }

 private:
  std::shared_ptr<Vertex<T>> origin;
  std::shared_ptr<HalfEdge<T>> twin;
  std::shared_ptr<HalfEdge<T>> next;
  std::shared_ptr<HalfEdge<T>> prev;
  std::shared_ptr<Face<T>> incident;
};
