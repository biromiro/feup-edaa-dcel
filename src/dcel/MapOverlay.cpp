//
// Created by nrtc on 5/20/2023.
//

#include "MapOverlay.h"

std::shared_ptr<DCEL<GeographicPoint>> MapOverlay::overlayDCELs(const std::shared_ptr<DCEL<GeographicPoint>> &dcel1,
                                                                const std::shared_ptr<DCEL<GeographicPoint>> &dcel2) {

    std::set<std::shared_ptr<Vertex<GeographicPoint>>, VertexComparator<GeographicPoint>> vertices;
    std::set<std::shared_ptr<HalfEdge<GeographicPoint>>, EdgeComparator<GeographicPoint>> edges;

    // still unsure faces should also be merged or only calculated after
    auto resultingDCEL = std::make_shared<DCEL<GeographicPoint>>();

    for (const auto& vertex: dcel1->getVertices()) {
        resultingDCEL->addVertex(vertex);
    }

    for (const auto& vertex: dcel2->getVertices()) {
        resultingDCEL->addVertex(vertex);
    }

    for (const auto& edge: dcel1->getEdges()) {
        resultingDCEL->addEdge(edge);
    }

    for (const auto& edge: dcel2->getEdges()) {
        resultingDCEL->addEdge(edge);
    }

    const auto intersections = LineSweep::findIntersections(resultingDCEL);

    std::cout << "The found intersection points were:" << std::endl << std::endl;

    for (const auto& intersection: intersections) {
        auto intersectionPoint = intersection->getIntersectionPoint();

        if (!dcel1->getVertex(intersectionPoint) || !dcel2->getVertex(intersectionPoint)) {
            std::cout << "Intersection point:" << std::endl;
            std::cout << intersectionPoint << std::endl << std::endl;

            std::cout << "Origin of intersecting edges:" << std::endl;
            for (const auto &edge: intersection->getEdges()) {
                std::cout << edge->getOrigin()->getValue() << std::endl;
            }

            std::cout << "------------------------------" << std::endl;

            std::shared_ptr<Vertex<GeographicPoint>> intersectionVertex = std::make_shared<Vertex<GeographicPoint>>(intersectionPoint);
            dcel1->addVertex(intersectionVertex);
            dcel2->addVertex(intersectionVertex);

            for (auto &edge : intersection->getEdges()) {
                std::shared_ptr<HalfEdge<GeographicPoint>> newHalfEdge1 = std::make_shared<HalfEdge<GeographicPoint>>(intersectionVertex);
                std::shared_ptr<HalfEdge<GeographicPoint>> newHalfEdge2 = std::make_shared<HalfEdge<GeographicPoint>>(intersectionVertex);

                edge->setNext(newHalfEdge2);
                edge->setTwin(newHalfEdge1);

                edge->getTwin()->setNext(newHalfEdge1);
                edge->getTwin()->setTwin(newHalfEdge2);

                newHalfEdge1->setPrev(edge);
                newHalfEdge2->setPrev(edge);

                if (dcel1->getVertex(edge->getOrigin()->getValue())) {
                    dcel1->addEdge(newHalfEdge1);
                    dcel1->addEdge(newHalfEdge2);
                } else {
                    dcel2->addEdge(newHalfEdge1);
                    dcel2->addEdge(newHalfEdge2);
                }
            }
        }
    }

    return nullptr;
}
