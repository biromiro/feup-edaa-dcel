//
// Created by nrtc on 5/17/2023.
//

#include "Parser.h"

std::vector<std::shared_ptr<HalfEdge<GeographicPoint>>>
Parser::genGeographicPoints(const std::shared_ptr<DCEL<GeographicPoint>>& dcel,
                            const json& json_file) {
  std::vector<std::shared_ptr<Vertex<GeographicPoint>>> vertices;
  std::vector<std::shared_ptr<HalfEdge<GeographicPoint>>> halfEdges, halfEdges2;

  for (auto coords : json_file[0]) {
    double longitude = coords[0], latitude = coords[1];

    auto geoPoint = GeographicPoint(latitude, longitude);
    auto vertex = dcel->getVertex(geoPoint);

    if (vertex == nullptr) {
      vertex = std::make_shared<Vertex<GeographicPoint>>(geoPoint);
      dcel->addVertex(vertex);
      vertices.emplace_back(vertex);
    } else {
      vertices.emplace_back(vertex);
    }
  }

  vertices.pop_back();

  if (isClockwise(vertices)) std::reverse(vertices.begin(), vertices.end());

  for (auto i = 0; i < vertices.size(); i++) {
    auto& currentVertex = vertices.at(i);
    auto& nextVertex = vertices.at((i + 1) % vertices.size());

    auto halfEdge1 = dcel->getHalfEdge(currentVertex, nextVertex);
    auto halfEdge2 = dcel->getHalfEdge(nextVertex, currentVertex);

    if (!halfEdge1 && !halfEdge2) {
      halfEdge1 = std::make_shared<HalfEdge<GeographicPoint>>(currentVertex);
      halfEdge2 = std::make_shared<HalfEdge<GeographicPoint>>(nextVertex);

      dcel->addEdge(halfEdge1);
      dcel->addEdge(halfEdge2);

      halfEdge1->setTwin(halfEdge2);
      halfEdge2->setTwin(halfEdge1);

    } else if (!halfEdge1 || !halfEdge2) {
      throw std::invalid_argument("Edges should've been created already.");
    }

    halfEdges.push_back(halfEdge1);
    halfEdges2.push_back(halfEdge2);
  }

  for (auto i = 0; i < halfEdges.size(); i++) {
    auto edge1 = halfEdges.at(i);
    auto edge2 = halfEdges.at((i + 1) % halfEdges.size());
    auto edge3 = halfEdges.at((i + 2) % halfEdges.size());

    if (edge2->getNext()) {
      edge2->getPrev()->setNext(edge1->getTwin());
      edge2->getNext()->setPrev(edge3->getTwin());
    }

    edge2->setNext(edge3);
    edge2->setPrev(edge1);
  }

  for (auto i = 0; i < halfEdges.size(); i++) {
    auto edge1 = halfEdges.at(i);
    auto edge2 = halfEdges.at((i + 1) % halfEdges.size());
    auto edge3 = halfEdges.at((i + 2) % halfEdges.size());

    if (!edge2->getNext()) {
      edge2->setNext(edge1);
      edge2->setPrev(edge3);
    }
  }

  return halfEdges;
}

void Parser::genFace(
    const std::shared_ptr<DCEL<GeographicPoint>>& dcel,
    const std::vector<std::shared_ptr<HalfEdge<GeographicPoint>>>& edges,
    const json& properties) {
  auto face = std::make_shared<Face<GeographicPoint>>(properties);
  dcel->addFace(face);

  face->setOuter(edges.at(0));

  for (const auto& edge : edges) edge->setIncident(face);
}

std::shared_ptr<DCEL<GeographicPoint>> Parser::parseJSONtoDCEL(
    const std::string& jsonFilePath) {
  // read json file
  std::ifstream f(jsonFilePath);
  json data = json::parse(f);

  // create DCEL
  auto dcel = std::make_shared<DCEL<GeographicPoint>>();

  if (data["type"] != "FeatureCollection") {
    const std::string error_msg =
        "The provided json file does not have the expected type "
        "'FeatureCollection'.";
    throw std::invalid_argument(error_msg);
  }

  // iterate through all vertices and create a vertex for each one
  std::cout << data["features"].size() << " features to parse." << std::endl;
  auto count = 1;
  for (json::iterator it = data["features"].begin();
       it != data["features"].end(); ++it) {
    auto feat = *it;

    if (feat["type"] != "Feature") {
      const std::string error_msg =
          "The provided json file does not have the expected type 'Feature'.";
      throw std::invalid_argument(error_msg);
    }

    auto geometry = feat["geometry"];

    if (geometry["type"] != "MultiPolygon" && geometry["type"] != "Polygon") {
      const std::string error_msg =
          "The provided json file does not have the expected type 'Polygon' or "
          "'MultiPolygon'.";
      throw std::invalid_argument(error_msg);
    }

    auto coords = geometry["coordinates"];
    if (geometry["type"] == "MultiPolygon") {
      auto counter = 1;
      for (auto& coord : coords) {
        auto edges = genGeographicPoints(dcel, coord);
        auto properties = feat["properties"];
        properties.emplace("MultiPolygonNumber", counter++);
        genFace(dcel, edges, properties);
      }

    } else if (geometry["type"] == "Polygon") {
      auto edges = genGeographicPoints(dcel, coords);
      genFace(dcel, edges, feat["properties"]);
    }

    std::cout << "Finished feature " << count++ << std::endl;
  }

  return dcel;
}
