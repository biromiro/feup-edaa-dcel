#pragma once

#include "face.h"
#include "vertex.h"

class HalfEdge {
public:
  HalfEdge();
  HalfEdge(HalfEdge &&) = default;
  HalfEdge(const HalfEdge &) = default;
  HalfEdge &operator=(HalfEdge &&) = default;
  HalfEdge &operator=(const HalfEdge &) = default;
  ~HalfEdge();

private:
  Vertex *origin;
  HalfEdge *twin;
  HalfEdge *next;
  HalfEdge *prev;
  Face *incident;
};
