// Copyright 2023 EDAA Group C
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "./dcel.hpp"

int main() {
  const auto* dcel = new DCEL<int>();

  std::ifstream f("../example.json");
  json data = json::parse(f);

  std::cout << data["happy"] << std::endl;

  std::cout << "Hello, World!" << std::endl;
  return 0;
}
