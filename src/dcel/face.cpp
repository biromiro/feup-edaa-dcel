#include "face.h"

Face::Face() {}

Face::~Face() {
  delete this->outer;
  for (auto edge : this->inner) {
    delete edge;
  }
}
