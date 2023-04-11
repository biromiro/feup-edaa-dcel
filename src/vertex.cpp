#include "vertex.h"
#include "edge.h"

Vertex::Vertex() {
  this->x = 0;
  this->y = 0;
  this->incident = new Edge();
}

Vertex::~Vertex() { delete this->incident; }

int Vertex::getX() { return this->x; }

int Vertex::getY() { return this->y; }
