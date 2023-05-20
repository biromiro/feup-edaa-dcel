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

    for (const auto& intersection: intersections) {
        bool inDCEL1 = false, inDCEL2 = false, intersectingDCELs = false;
        for (const auto& edge: intersection->getEdges()) {
            auto edgeDCEL1 = dcel1->getHalfEdge(edge->getOrigin(), edge->getTwin()->getOrigin()),
            edgeDCEL2 = dcel2->getHalfEdge(edge->getOrigin(), edge->getTwin()->getOrigin());

            if (edgeDCEL1) inDCEL1 = true;
            if (edgeDCEL2) inDCEL2 = true;

            if (inDCEL1 && inDCEL2){
                intersectingDCELs = true;
                break;
            }
        }

        if (!intersectingDCELs) continue;

        auto vertexDCEL = resultingDCEL->getVertex(intersection->getIntersectionPoint());
        auto intersectionPoint = intersection->getIntersectionPoint();


        if (vertexDCEL) {
            std::vector<std::shared_ptr<HalfEdge<GeographicPoint>>> cyclicOrder;
            std::shared_ptr<HalfEdge<GeographicPoint>> halfEdge;
            for (const auto& edge: intersection->getEdges()) {
                cyclicOrder.push_back(edge);
                cyclicOrder.push_back(edge->getTwin());

                if (!(edge->getOrigin()->getValue() == intersectionPoint ||
                    edge->getTwin()->getOrigin()->getValue() == intersectionPoint)){

                    halfEdge = edge;
                }

            }

            auto twinEdge = halfEdge->getTwin();

            // create two new half edges
            auto newEdge1 = std::make_shared<HalfEdge<GeographicPoint>>(),
                    newEdge2 = std::make_shared<HalfEdge<GeographicPoint>>();

            newEdge1->setOrigin(vertexDCEL);
            newEdge2->setOrigin(vertexDCEL);

            halfEdge->setTwin(newEdge1);
            twinEdge->setTwin(newEdge2);

            newEdge1->setTwin(halfEdge);
            newEdge2->setTwin(twinEdge);

            resultingDCEL->addEdge(newEdge1);
            resultingDCEL->addEdge(newEdge2);

            newEdge1->setNext(twinEdge->getNext());
            newEdge2->setNext(halfEdge->getNext());

            twinEdge->getNext()->setPrev(newEdge1);
            halfEdge->getNext()->setPrev(newEdge2);

            cyclicOrder.push_back(newEdge1); cyclicOrder.push_back(newEdge2);

            cyclicOrderingOfEdges(intersectionPoint, cyclicOrder);

            int idx = std::distance(
                    cyclicOrder.begin(),
                    std::find(cyclicOrder.begin(), cyclicOrder.end(), halfEdge)
                    );

            cyclicOrder.at(idx)->setNext(cyclicOrder.at((idx - 1) % cyclicOrder.size()));
            cyclicOrder.at((idx - 1) % cyclicOrder.size())->setPrev(cyclicOrder.at(idx));

            idx = std::distance(
                    cyclicOrder.begin(),
                    std::find(cyclicOrder.begin(), cyclicOrder.end(), twinEdge)
            );

            cyclicOrder.at(idx)->setNext(cyclicOrder.at((idx - 1) % cyclicOrder.size()));
            cyclicOrder.at((idx - 1) % cyclicOrder.size())->setPrev(cyclicOrder.at(idx));

            idx = std::distance(
                    cyclicOrder.begin(),
                    std::find(cyclicOrder.begin(), cyclicOrder.end(), newEdge1)
            );

            cyclicOrder.at(idx)->setNext(cyclicOrder.at((idx + 1) % cyclicOrder.size()));
            cyclicOrder.at((idx + 1) % cyclicOrder.size())->setPrev(cyclicOrder.at(idx));

            idx = std::distance(
                    cyclicOrder.begin(),
                    std::find(cyclicOrder.begin(), cyclicOrder.end(), newEdge2)
            );

            cyclicOrder.at(idx)->setNext(cyclicOrder.at((idx + 1) % cyclicOrder.size()));
            cyclicOrder.at((idx + 1) % cyclicOrder.size())->setPrev(cyclicOrder.at(idx));

        } else {
            auto vertex = std::make_shared<Vertex<GeographicPoint>>(intersectionPoint);
            resultingDCEL->addVertex(vertex);

            std::vector<std::shared_ptr<HalfEdge<GeographicPoint>>> cyclicOrder;
            std::vector<std::shared_ptr<HalfEdge<GeographicPoint>>> newEdges;

            for (const auto& edge: intersection->getEdges()) {
                auto twinEdge = edge->getTwin();
                // create two new half edges
                auto newEdge1 = std::make_shared<HalfEdge<GeographicPoint>>(),
                        newEdge2 = std::make_shared<HalfEdge<GeographicPoint>>();

                newEdge1->setOrigin(vertex);
                newEdge2->setOrigin(vertex);

                edge->setTwin(newEdge1);
                twinEdge->setTwin(newEdge2);

                newEdge1->setTwin(edge);
                newEdge2->setTwin(twinEdge);

                resultingDCEL->addEdge(newEdge1);
                resultingDCEL->addEdge(newEdge2);

                newEdge1->setNext(twinEdge->getNext());
                newEdge2->setNext(edge->getNext());

                twinEdge->getNext()->setPrev(newEdge1);
                edge->getNext()->setPrev(newEdge2);

                cyclicOrder.push_back(edge);
                cyclicOrder.push_back(twinEdge);
                cyclicOrder.push_back(newEdge1);
                cyclicOrder.push_back(newEdge2);

                newEdges.push_back(newEdge1);
                newEdges.push_back(newEdge2);
            }

            std::cout << "\nNew Ordering of edges at " << intersectionPoint << std::endl;
            cyclicOrderingOfEdges(intersectionPoint, cyclicOrder);
            for (const auto& edge: cyclicOrder) {
                std::cout << "Edge from " << edge->getOrigin()->getValue() << " to " << edge->getTwin()->getOrigin()->getValue() << std::endl;
            }

            for (const auto& newEdge: newEdges) {
                int idx = std::distance(
                        cyclicOrder.begin(),
                        std::find(cyclicOrder.begin(), cyclicOrder.end(), newEdge)
                );

                cyclicOrder.at(idx)->setPrev(cyclicOrder.at((idx + 1) % cyclicOrder.size()));
                cyclicOrder.at((idx + 1) % cyclicOrder.size())->setNext(cyclicOrder.at(idx));
            }
        }
    }

    return resultingDCEL;
}

void MapOverlay::cyclicOrderingOfEdges(GeographicPoint intersectionPoint, std::vector<std::shared_ptr<HalfEdge<GeographicPoint>>> &edges) {
    std::sort(edges.begin(), edges.end(), [intersectionPoint](
            const std::shared_ptr<HalfEdge<GeographicPoint>>& h1,
            const std::shared_ptr<HalfEdge<GeographicPoint>>& h2) {

        if (h1->getTwin() == h2){
            return h1->getOrigin()->getValue() == intersectionPoint;
        }

        auto a = h1->getOrigin()->getValue() == intersectionPoint ?  h1->getTwin()->getOrigin()->getValue() : h1->getOrigin()->getValue();
        auto b = h2->getOrigin()->getValue() == intersectionPoint ?  h2->getTwin()->getOrigin()->getValue() : h2->getOrigin()->getValue();

        // Computes the quadrant for a and b (0-3):
        //     ^
        //   1 | 0
        //  ---+-->
        //   2 | 3

        const int dax = ((a.getLongitude() - intersectionPoint.getLongitude()) > 0) ? 1 : 0;
        const int day = ((a.getLatitude() - intersectionPoint.getLatitude()) > 0) ? 1 : 0;
        const int qa = (1 - dax) + (1 - day) + ((dax & (1 - day)) << 1);

        /* The previous computes the following:

           const int qa =
           (  (a.x() > center.x())
            ? ((a.y() > center.y())
                ? 0 : 3)
            : ((a.y() > center.y())
                ? 1 : 2)); */

        const int dbx = ((b.getLongitude() - intersectionPoint.getLongitude()) > 0) ? 1 : 0;
        const int dby = ((b.getLatitude() - intersectionPoint.getLatitude()) > 0) ? 1 : 0;
        const int qb = (1 - dbx) + (1 - dby) + ((dbx & (1 - dby)) << 1);

        if (qa == qb) {
            return (b.getLongitude() - intersectionPoint.getLongitude()) * (a.getLatitude() - intersectionPoint.getLatitude()) < (b.getLatitude() - intersectionPoint.getLatitude()) * (a.getLongitude() - intersectionPoint.getLongitude());
        }

        return qa < qb;
    });
}
