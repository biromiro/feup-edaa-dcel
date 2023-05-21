// Copyright 2023 EDAA Group C
#pragma once

#include <memory>
#include "../json.hpp"
#include <set>
#include <utility>
#include <vector>

#include "./Vertex.hpp"

template <class T>
class Face {
 public:
  Face() : outer(), inner(), property() {}
  explicit Face(nlohmann::json property)
      : outer(), inner(), property(std::move(property)) {}
  Face(Face<T> &&) = default;
  Face(const Face<T> &) = default;
  Face<T> &operator=(Face<T> &&) = default;
  Face<T> &operator=(const Face<T> &) = default;

  [[nodiscard]] const nlohmann::json &getProperty() const { return property; }

  const std::shared_ptr<HalfEdge<T>> &getOuter() const { return outer; }

  void setOuter(const std::shared_ptr<HalfEdge<T>> &outer_) {
    this->outer = outer_;
  }

  bool operator<(const Face &rhs) const {
    return property.dump() < rhs.property.dump();
  }

 private:
  nlohmann::json property;
  std::shared_ptr<HalfEdge<T>> outer;
  std::set<std::shared_ptr<HalfEdge<T>>> inner;
};
