#include "dcel.h"
#include "face.h"
#include "half_edge.h"
#include "vertex.h"
#include <queue>
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

void DCEL::mergeDCEL(DCEL s1, DCEL s2) {
  this->getVertices().clear();
  this->getVertices().reserve(s1.getVertices().size() +
                              s2.getVertices().size());

  this->getVertices().insert(this->getVertices().end(),
                             s1.getVertices().begin(), s1.getVertices().end());

  this->getVertices().insert(this->getVertices().end(),
                             s2.getVertices().begin(), s2.getVertices().end());

  this->getEdges().clear();
  this->getEdges().reserve(s1.getEdges().size() + s2.getEdges().size());

  this->getEdges().insert(this->getEdges().end(), s1.getEdges().begin(),
                          s1.getEdges().end());

  this->getEdges().insert(this->getEdges().end(), s2.getEdges().begin(),
                          s2.getEdges().end());

  this->getFaces().clear();

  return;
}

// TODO: Finish this
std::vector<Vertex> findIntersectionPoints(DCEL d) {
  std::queue<Vertex> q;

  for (Vertex v : d.getVertices()) {
    q.push(v);
  }

  while (!q.empty()) {
    Vertex v = q.front();
    q.pop();
  }

  return std::vector<Vertex>();
}
