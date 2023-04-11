#pragma once

class Edge {
public:
  Edge();
  Edge(Edge &&) = default;
  Edge(const Edge &) = default;
  Edge &operator=(Edge &&) = default;
  Edge &operator=(const Edge &) = default;
  ~Edge();

private:
};
