//
// Created by nrtc on 5/21/2023.
//

#include "Cycle.h"

Cycle::Cycle(bool isUnbounded) {
    this->cycleVec = std::vector<std::shared_ptr<HalfEdge<GeographicPoint>>>();
    this->unboundedCycle = isUnbounded;
}

const std::vector<std::shared_ptr<HalfEdge<GeographicPoint>>> &Cycle::getCycle() const {
    return cycleVec;
}

void Cycle::addEdgeToCycle(const std::shared_ptr<HalfEdge<GeographicPoint>> & edge) {
    cycleVec.push_back(edge);

}

std::shared_ptr<Vertex<GeographicPoint>> Cycle::getLeftmostVertex() const {
    std::shared_ptr<Vertex<GeographicPoint>> leftmostVertex = nullptr;
    for (const auto& edge: cycleVec) {
        if (leftmostVertex == nullptr) leftmostVertex = edge->getOrigin();
        else {
            if (edge->getOrigin()->getValue().getLongitude() < leftmostVertex->getValue().getLongitude())
                leftmostVertex = edge->getOrigin();
            else if (leftmostVertex->getValue().getLongitude() == edge->getOrigin()->getValue().getLongitude()){
                if (edge->getOrigin()->getValue().getLatitude() < leftmostVertex->getValue().getLatitude())
                    leftmostVertex = edge->getOrigin();
            }
        }
    }
    return leftmostVertex;
}

void Cycle::reverseCycle() {
    std::reverse(cycleVec.begin(), cycleVec.end());
}

double Cycle::isBoundaryCycle() const{
    std::vector<std::shared_ptr<Vertex<GeographicPoint>>> vertices;
    for (const auto& edge: cycleVec){
        vertices.push_back(edge->getOrigin());
    }
    return !isClockwise(vertices);
}

void Cycle::setID(size_t id) {
    Cycle::id = id;
}

size_t Cycle::getID() const {
    return id;
}

bool Cycle::isUnboundedCycle() const{
    return unboundedCycle;
}

std::set<std::shared_ptr<Face<GeographicPoint>>> Cycle::getFaces() {
    std::set<std::shared_ptr<Face<GeographicPoint>>> faces;
    for (const auto& edge: cycleVec)
        faces.insert(edge->getIncident());
    return faces;
}
