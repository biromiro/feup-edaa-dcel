#pragma once

#include "edge.h"
class Vertex {
public:
  Vertex();
  Vertex(Vertex &&) = default;
  Vertex(const Vertex &) = default;
  Vertex &operator=(Vertex &&) = default;
  Vertex &operator=(const Vertex &) = default;
  ~Vertex();

  int getX();
  int getY();

private:
  int x, y;
  Edge *incident;
};
