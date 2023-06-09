// Copyright 2023 EDAA Group C
#pragma once

#include <memory>
#include "../json.hpp"
#include <set>
#include <utility>
#include <vector>

#include "./Vertex.tpp"

template <class T>
class Face {
 public:
  Face() : outer(), inner(), property({}) {}
  explicit Face(nlohmann::json property)
      : outer(), inner(), property(std::move(property)) {}
  Face(Face<T> &&) = default;
  Face(const Face<T> &) = default;
  Face<T> &operator=(Face<T> &&) = default;
  Face<T> &operator=(const Face<T> &) = default;

  [[nodiscard]] nlohmann::json getProperty() const {
      return property;
  }

  const std::shared_ptr<HalfEdge<T>> &getOuter() const { return outer; }

  void setOuter(const std::shared_ptr<HalfEdge<T>> &outer_) {
    this->outer = outer_;
  }

  void addInner(const std::shared_ptr<HalfEdge<T>>& edge) {
      inner.insert(edge);
  }

  bool operator<(const Face &rhs) const {
    return property.dump() < rhs.property.dump();
  }

  void addProperty(nlohmann::json newProperty) {
      if (!newProperty.is_null()){
          for (nlohmann::json::iterator it = newProperty.begin(); it != newProperty.end(); ++it) {
              auto key = it.key(); auto value = it.value();
              if (!property[key].is_null()) {
                  if (property[key].is_array()) {
                      property[key].push_back(value);
                  } else {
                      nlohmann::json arr;
                      arr.push_back(value);
                      arr.push_back(property[key]);
                      property[key] = arr;
                  }
              } else property[key] = value;
          }
      }
  }
 private:
  nlohmann::json property;
  std::shared_ptr<HalfEdge<T>> outer;
  std::set<std::shared_ptr<HalfEdge<T>>> inner;
};
