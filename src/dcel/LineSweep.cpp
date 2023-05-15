// Copyright (c) 2023 EDAA Group C. All Rights Reserved.

#include "./LineSweep.h"

#include <vector>

#include "GeographicPoint.h"

struct PointComparator {
  bool operator()(const Vertex<GeographicPoint> lhs,
                  Vertex<GeographicPoint> rhs) const {
    if (lhs.getValue().getLatitude() == rhs.getValue().getLatitude()) {
      return lhs.getValue().getLongitude() > rhs.getValue().getLongitude();
    }
    return lhs.getValue().getLatitude() < rhs.getValue().getLatitude();
  }
};

/**
 * I need to first unite the half edges into line segments, but I need to do so
 * without increasing the space complexity
 */

std::vector<Vertex<GeographicPoint>> findIntersection(
    std::vector<HalfEdge<GeographicPoint>> segments) {
  std::vector<Vertex<GeographicPoint>> vertices;

  for (const auto segment : segments) {
    vertices.push_back(*segment.getOrigin());
  }

  std::priority_queue<Vertex<GeographicPoint>,
                      std::vector<Vertex<GeographicPoint>>, PointComparator>
      queue(vertices.begin(), vertices.end());

  // TODO(Tiago): Figure out a BST to store the edges

  while (!queue.empty()) {
    std::cout << queue.top().getValue().getLatitude() << " "
              << queue.top().getValue().getLongitude() << std::endl;
    queue.pop();
  }

  return vertices;
}
