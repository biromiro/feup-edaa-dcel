#pragma once

#include "half_edge.h"
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
  HalfEdge *getIncident();

private:
  int x, y;
  HalfEdge *incident;
};
