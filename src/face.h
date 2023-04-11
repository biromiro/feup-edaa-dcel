#pragma once

#include "vertex.h"
#include <vector>

class Face {
public:
  Face();
  Face(Face &&) = default;
  Face(const Face &) = default;
  Face &operator=(Face &&) = default;
  Face &operator=(const Face &) = default;
  ~Face();

private:
  HalfEdge *outer;
  std::vector<HalfEdge *> inner;
};
