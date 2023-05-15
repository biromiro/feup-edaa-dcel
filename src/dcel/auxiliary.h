#pragma once

#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "./dcel.hpp"
#include "./half_edge.hpp"
#include "GeographicPoint.h"

using json = nlohmann::json;

bool isClockwise(
    const std::vector<std::shared_ptr<Vertex<GeographicPoint>>>& vertices);

std::vector<std::shared_ptr<HalfEdge<GeographicPoint>>> genGeographicPoints(
    const std::shared_ptr<DCEL<GeographicPoint>>& dcel, const json& json_file);

void genFace(
    const std::shared_ptr<DCEL<GeographicPoint>>& dcel,
    const std::vector<std::shared_ptr<HalfEdge<GeographicPoint>>>& edges,
    const json& properties);

std::shared_ptr<DCEL<GeographicPoint>> parseJSONtoDCEL(
    const std::string& jsonFilePath);
