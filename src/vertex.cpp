#include "vertex.h"
#include "half_edge.h"

Vertex::Vertex() {
  this->x = 0;
  this->y = 0;
  this->incident = new HalfEdge();
}

Vertex::~Vertex() { delete this->incident; }

int Vertex::getX() { return this->x; }

int Vertex::getY() { return this->y; }

HalfEdge *Vertex::getIncident() { return this->incident; }
