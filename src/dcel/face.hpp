// Copyright 2023 EDAA Group C
#pragma once

#include <vector>

#include "./vertex.hpp"

template <class T>
class Face {
 public:
  Face() : outer(), inner() {}
  Face(Face<T> &&) = default;
  Face(const Face<T> &) = default;
  Face<T> &operator=(Face<T> &&) = default;
  Face<T> &operator=(const Face<T> &) = default;

  ~Face() {
    delete this->outer;
    for (auto edge : this->inner) {
      delete edge;
    }
  }

 private:
  HalfEdge<T> *outer;
  std::vector<HalfEdge<T> *> inner;
};
