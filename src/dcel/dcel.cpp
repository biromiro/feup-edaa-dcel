#include "dcel.h"
#include "face.h"
#include "half_edge.h"
#include "vertex.h"
#include <vector>

DCEL::DCEL() {
  this->vertices = std::vector<Vertex>();
  this->edges = std::vector<HalfEdge>();
  this->faces = std::vector<Face>();
}

DCEL::~DCEL() {}

std::vector<HalfEdge> DCEL::getEdges() { return this->edges; }

std::vector<Face> DCEL::getFaces() { return this->faces; }

std::vector<Vertex> DCEL::getVertices() { return this->vertices; }
