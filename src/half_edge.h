#pragma once

class HalfEdge {
public:
  HalfEdge();
  HalfEdge(HalfEdge &&) = default;
  HalfEdge(const HalfEdge &) = default;
  HalfEdge &operator=(HalfEdge &&) = default;
  HalfEdge &operator=(const HalfEdge &) = default;
  ~HalfEdge();

private:
};
