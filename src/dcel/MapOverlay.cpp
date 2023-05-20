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

    std::cout << "The found intersection points were:" << std::endl;

    for (const auto& intersection: intersections) {
        auto vertex = dcel1->getVertex(intersection->getIntersectionPoint());
        auto vertex2 = dcel2->getVertex(intersection->getIntersectionPoint());
        if (!vertex && !vertex2)
            std::cout << intersection->getIntersectionPoint() << std::endl;
    }

    return nullptr;
}
