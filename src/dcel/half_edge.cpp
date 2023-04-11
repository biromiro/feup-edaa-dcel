#include "half_edge.h"

HalfEdge::HalfEdge() {
  this->incident = 0;
  this->next = 0;
  this->prev = 0;
  this->twin = 0;
  this->origin = 0;
}

HalfEdge::~HalfEdge() {
  delete this->incident;
  delete this->next;
  delete this->prev;
  delete this->twin;
  delete this->origin;
}
