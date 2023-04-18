// Copyright 2023 EDAA Group C
#pragma once

#include <vector>
#include <set>

#include "./vertex.hpp"

template <class T>
class Face {
 public:
  Face() : outer(), inner() {}
  Face(Face<T> &&) = default;
  Face(const Face<T> &) = default;
  Face<T> &operator=(Face<T> &&) = default;
  Face<T> &operator=(const Face<T> &) = default;

private:
  std::shared_ptr<HalfEdge<T>> outer;
  std::set<std::shared_ptr<HalfEdge<T>>> inner;
};
