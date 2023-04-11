#include "face.h"
#include <vector>

Face::Face() {
  this->outer = 0;
  this->inner = std::vector<HalfEdge *>();
}

Face::~Face() {
  delete this->outer;
  for (auto edge : this->inner) {
    delete edge;
  }
}
