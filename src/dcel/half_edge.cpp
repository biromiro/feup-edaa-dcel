#include "half_edge.h"

HalfEdge::HalfEdge() {}

HalfEdge::~HalfEdge() {
  delete this->incident;
  delete this->next;
  delete this->prev;
  delete this->twin;
  delete this->origin;
}
