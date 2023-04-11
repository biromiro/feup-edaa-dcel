#pragma once

#include "face.h"
#include "vertex.h"
#include <vector>

class DCEL {
public:
  DCEL();
  DCEL(DCEL &&) = default;
  DCEL(const DCEL &) = default;
  DCEL &operator=(DCEL &&) = default;
  DCEL &operator=(const DCEL &) = default;
  ~DCEL();

  std::vector<Face> getFaces();
  std::vector<HalfEdge> getEdges();
  std::vector<Vertex> getVertices();

private:
  std::vector<Face> faces;
  std::vector<HalfEdge> edges;
  std::vector<Vertex> vertices;

  void mergeDCEL(DCEL s1, DCEL s2);
};
