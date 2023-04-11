#pragma once

class HalfEdge;

class Vertex {
public:
  Vertex();
  Vertex(Vertex &&) = default;
  Vertex(const Vertex &) = default;
  Vertex &operator=(Vertex &&) = default;
  Vertex &operator=(const Vertex &) = default;
  ~Vertex();

  [[nodiscard]] long long int getX() const;
  [[nodiscard]] long long int getY() const;
  HalfEdge *getIncident();

private:
  long long int x, y;
  HalfEdge *incident;
};
