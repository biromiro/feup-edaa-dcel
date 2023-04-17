#pragma once

#include "vertex.hpp"
#include <vector>
#include <set>

template <class T>
class Face {
public:
  Face() : outer(), inner() {};
  Face(Face<T> &&) = default;
  Face(const Face<T> &) = default;
  Face<T> &operator=(Face<T> &&) = default;
  Face<T> &operator=(const Face<T> &) = default;
  ~Face() {
      delete this->outer;
      for (auto edge : this->inner) {
          delete edge;
      }
  };

private:
  std::shared_ptr<HalfEdge<T>> outer;
  std::set<std::shared_ptr<HalfEdge<T>>> inner;
};
