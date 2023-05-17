// Copyright (c) 2023 EDAA Group C. All Rights Reserved.
#pragma once

#include <iostream>
#include <queue>
#include <vector>

#include "./half_edge.hpp"
#include "./vertex.hpp"
#include "GeographicPoint.h"
#include "Event.h"

std::vector<Vertex<GeographicPoint>> findIntersection(
    std::vector<HalfEdge<GeographicPoint>> vertices);
