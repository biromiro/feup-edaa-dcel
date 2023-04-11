#include "vertex.h"
#include "half_edge.h"

Vertex::Vertex() {
  this->x = 0;
  this->y = 0;
  this->incident = new HalfEdge();
}

Vertex::~Vertex() { delete this->incident; }

long long int Vertex::getX() const { return this->x; }

long long int Vertex::getY() const { return this->y; }

HalfEdge *Vertex::getIncident() { return this->incident; }
