#include "../dcel/auxiliary/geographic_point/GeographicPoint.h"
#include "../dcel/dcel.tpp"

class Visualizer {
 public:
  explicit Visualizer(DCEL<GeographicPoint> dcel);

 private:
  DCEL<GeographicPoint> dcel;
};
