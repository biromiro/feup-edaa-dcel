#include <memory>
#include <random>

#include "../dcel/auxiliary/geographic_point/GeographicPoint.h"
#include "../dcel/dcel.tpp"
#include "matplot/util/keywords.h"

class Visualizer {
 public:
  explicit Visualizer(std::shared_ptr<DCEL<GeographicPoint>> dcel);

  void plotDCEL(const bool hold = matplot::on);

  void setDCEL(std::shared_ptr<DCEL<GeographicPoint>> dcel);

  void showDCEL();

 private:
  std::shared_ptr<DCEL<GeographicPoint>> dcel;
  std::mt19937_64 rng;
  std::uniform_real_distribution<double> unif;

  float genRandom();
};
