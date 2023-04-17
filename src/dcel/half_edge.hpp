// Copyright 2023 EDAA Group C
#pragma once

template <class T>
class Vertex;

template <class T>
class Face;

template <class T>
class HalfEdge {
 public:
  HalfEdge() : incident(), next(), prev(), twin(), origin() {}
  HalfEdge(HalfEdge &&) = default;
  HalfEdge(const HalfEdge &) = default;
  HalfEdge &operator=(HalfEdge &&) = default;
  HalfEdge &operator=(const HalfEdge &) = default;
  ~HalfEdge() {
    delete this->incident;
    delete this->next;
    delete this->prev;
    delete this->twin;
    delete this->origin;
  }

 private:
  Vertex<T> *origin;
  HalfEdge<T> *twin;
  HalfEdge<T> *next;
  HalfEdge<T> *prev;
  Face<T> *incident;
};
