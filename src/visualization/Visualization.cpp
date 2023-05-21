// Copyright (c) 2023 EDAA Group C. All Rights Reserved.
#include "./Visualization.h"

#include <matplot/matplot.h>
#include <time.h>

#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>
#include <random>

Visualizer::Visualizer(std::shared_ptr<DCEL<GeographicPoint>> dcel) {
  this->dcel = dcel;
  std::mt19937_64 rng;
  // initialize the random number generator with time-dependent seed
  uint64_t timeSeed =
      std::chrono::high_resolution_clock::now().time_since_epoch().count();
  std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32)};
  rng.seed(ss);
  // initialize a uniform distribution between 0 and 1
  this->unif = std::uniform_real_distribution<double>(0, 1);
}

float Visualizer::genRandom() { return unif(this->rng); }

void Visualizer::plotDCEL(const bool hold) {
  auto colorMap = std::map<nlohmann::json, std::array<float, 4>>();
  srand((unsigned)time(NULL));

  for (const auto& face : this->dcel->getFaces()) {
    if (face == dcel->getUnboundedFace()) continue;

    matplot::vector_1d xVals = {};
    matplot::vector_1d yVals = {};

    auto originEdge = face->getOuter();

    auto faceProperty = face->getProperty();
    auto property = nlohmann::to_string(faceProperty);

    xVals.push_back(originEdge->getOrigin()->getValue().getLongitude());
    yVals.push_back(originEdge->getOrigin()->getValue().getLatitude());

    auto nextEdge = originEdge->getNext();

    xVals.push_back(nextEdge->getOrigin()->getValue().getLongitude());
    yVals.push_back(nextEdge->getOrigin()->getValue().getLatitude());

    while (nextEdge->getOrigin() != originEdge->getOrigin()) {
      nextEdge = nextEdge->getNext();
      xVals.push_back(nextEdge->getOrigin()->getValue().getLongitude());
      yVals.push_back(nextEdge->getOrigin()->getValue().getLatitude());
    }

    auto plot = matplot::plot(xVals, yVals);
    plot->fill(true);

    auto colorPair = colorMap.find(faceProperty);
    std::array<float, 4> colors;
    if (colorPair != colorMap.end()) {
      colors = colorPair->second;
    } else {
      colors = {genRandom(), genRandom(), genRandom(), 1.f};
    }

    plot->color(colors);
    plot->display_name(property);
    matplot::hold(matplot::on);
  }

  matplot::legend();
  matplot::hold(hold);
}

void Visualizer::setDCEL(std::shared_ptr<DCEL<GeographicPoint>> dcel) {
  this->dcel = dcel;
}

void Visualizer::showDCEL() {
  matplot::axis(matplot::equal);
  matplot::show();
}
