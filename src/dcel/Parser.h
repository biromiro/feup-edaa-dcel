//
// Created by nrtc on 5/17/2023.
//

#ifndef DCEL_PARSER_H
#define DCEL_PARSER_H

#pragma once

#include <memory>
#include "json.hpp"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include "./dcel.hpp"
#include "./half_edge.hpp"
#include "auxiliary.h"
#include "GeographicPoint.h"

using json = nlohmann::json;

class Parser {
public:
    static std::vector<std::shared_ptr<HalfEdge<GeographicPoint>>> genGeographicPoints(
            const std::shared_ptr<DCEL<GeographicPoint>>& dcel, const json& json_file);

    static void genFace(
            const std::shared_ptr<DCEL<GeographicPoint>>& dcel,
            const std::vector<std::shared_ptr<HalfEdge<GeographicPoint>>>& edges,
            const json& properties);

    static std::shared_ptr<DCEL<GeographicPoint>> parseJSONtoDCEL(
            const std::string& jsonFilePath);
};


#endif //DCEL_PARSER_H
