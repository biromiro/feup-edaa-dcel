//
// Created by nrtc on 5/20/2023.
//

#include "MapOverlay.h"
#include "../../auxiliary/cycle_graph/Cycle.h"
#include "../../auxiliary/cycle_graph/auxiliary/Graph.tpp"
#include "../../auxiliary/cycle_graph/connectivity/Connectivity.tpp"

std::shared_ptr<DCEL<GeographicPoint>> MapOverlay::overlayDCELs(const std::shared_ptr<DCEL<GeographicPoint>> &dcel1,
                                                                const std::shared_ptr<DCEL<GeographicPoint>> &dcel2) {

    std::set<std::shared_ptr<Vertex<GeographicPoint>>, VertexComparator<GeographicPoint>> vertices;
    std::set<std::shared_ptr<HalfEdge<GeographicPoint>>, EdgeComparator<GeographicPoint>> edges;

    auto dcel = mergeDCELs(dcel1, dcel2);

    generateFaces(dcel);

    return dcel;
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


        const int dax = ((a.getLongitude() - intersectionPoint.getLongitude()) > 0) ? 1 : 0;
        const int day = ((a.getLatitude() - intersectionPoint.getLatitude()) > 0) ? 1 : 0;
        const int qa = (1 - dax) + (1 - day) + ((dax & (1 - day)) << 1);

        const int dbx = ((b.getLongitude() - intersectionPoint.getLongitude()) > 0) ? 1 : 0;
        const int dby = ((b.getLatitude() - intersectionPoint.getLatitude()) > 0) ? 1 : 0;
        const int qb = (1 - dbx) + (1 - dby) + ((dbx & (1 - dby)) << 1);

        if (qa == qb) {
            return (b.getLongitude() - intersectionPoint.getLongitude()) * (a.getLatitude() - intersectionPoint.getLatitude()) < (b.getLatitude() - intersectionPoint.getLatitude()) * (a.getLongitude() - intersectionPoint.getLongitude());
        }

        return qa < qb;
    });
}


std::shared_ptr<DCEL<GeographicPoint>> MapOverlay::mergeDCELs(const std::shared_ptr<DCEL<GeographicPoint>> &dcel1,
                                                              const std::shared_ptr<DCEL<GeographicPoint>> &dcel2) {

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
            vertexDCEL->setLeftEdge(intersection->getLeftEdge());

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

            newEdge1->setIncident(twinEdge->getIncident());
            newEdge2->setIncident(halfEdge->getIncident());

            cyclicOrder.push_back(newEdge1); cyclicOrder.push_back(newEdge2);

            cyclicOrderingOfEdges(intersectionPoint, cyclicOrder);

            int idx = std::distance(
                    cyclicOrder.begin(),
                    std::find(cyclicOrder.begin(), cyclicOrder.end(), halfEdge)
            );

            cyclicOrder.at(idx)->setNext(cyclicOrder.at((idx + 1) % cyclicOrder.size()));
            cyclicOrder.at((idx + 1) % cyclicOrder.size())->setPrev(cyclicOrder.at(idx));

            idx = std::distance(
                    cyclicOrder.begin(),
                    std::find(cyclicOrder.begin(), cyclicOrder.end(), twinEdge)
            );

            cyclicOrder.at(idx)->setNext(cyclicOrder.at((idx + 1) % cyclicOrder.size()));
            cyclicOrder.at((idx + 1) % cyclicOrder.size())->setPrev(cyclicOrder.at(idx));

            idx = std::distance(
                    cyclicOrder.begin(),
                    std::find(cyclicOrder.begin(), cyclicOrder.end(), newEdge1)
            );

            cyclicOrder.at(idx)->setNext(cyclicOrder.at((idx - 1) % cyclicOrder.size()));
            cyclicOrder.at((idx - 1) % cyclicOrder.size())->setPrev(cyclicOrder.at(idx));

            idx = std::distance(
                    cyclicOrder.begin(),
                    std::find(cyclicOrder.begin(), cyclicOrder.end(), newEdge2)
            );

            cyclicOrder.at(idx)->setNext(cyclicOrder.at((idx - 1) % cyclicOrder.size()));
            cyclicOrder.at((idx - 1) % cyclicOrder.size())->setPrev(cyclicOrder.at(idx));

        } else {
            auto vertex = std::make_shared<Vertex<GeographicPoint>>(intersectionPoint);
            vertex->setLeftEdge(intersection->getLeftEdge());

            resultingDCEL->addVertex(vertex);

            std::vector<std::shared_ptr<HalfEdge<GeographicPoint>>> cyclicOrder;
            std::vector<std::shared_ptr<HalfEdge<GeographicPoint>>> newEdges;

            for (const auto& edgePair: intersection->getEdgePairs()) {

                auto first = edgePair.first, second = edgePair.second;
                std::shared_ptr<HalfEdge<GeographicPoint>> edge;

                if (isBetween(first->getOrigin()->getValue(), intersectionPoint, first->getTwin()->getOrigin()->getValue()))
                    edge = first;
                else edge = second;

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

                newEdge1->setIncident(twinEdge->getIncident());
                newEdge2->setIncident(edge->getIncident());

                cyclicOrder.push_back(edge);
                cyclicOrder.push_back(twinEdge);
                cyclicOrder.push_back(newEdge1);
                cyclicOrder.push_back(newEdge2);

                newEdges.push_back(newEdge1);
                newEdges.push_back(newEdge2);
            }

            cyclicOrderingOfEdges(intersectionPoint, cyclicOrder);

            for (const auto& newEdge: newEdges) {
                int idx = std::distance(
                        cyclicOrder.begin(),
                        std::find(cyclicOrder.begin(), cyclicOrder.end(), newEdge)
                );

                cyclicOrder.at(idx)->setPrev(cyclicOrder.at((idx - 1) % cyclicOrder.size()));
                cyclicOrder.at((idx - 1) % cyclicOrder.size())->setNext(cyclicOrder.at(idx));
            }
        }
    }

    return resultingDCEL;
}


void MapOverlay::generateFaces(std::shared_ptr<DCEL<GeographicPoint>> &dcel) {

    auto visitedSet = std::map<std::shared_ptr<HalfEdge<GeographicPoint>>, size_t>();
    for (const auto& edge: dcel->getEdges())
        visitedSet.insert(std::make_pair(edge, 0));


    auto cycles = std::vector<Cycle>();
    size_t cycleNum = 1;

    for (auto& edge: visitedSet) {
        if (edge.second != 0) continue;
        auto cycle = Cycle();

        auto current = edge.first;

        do {
            cycle.addEdgeToCycle(current);
            auto itr = visitedSet.find(current);
            if (itr->second) {
                std::cout << "did not cycle" << std::endl; break;
            }
            itr->second = cycleNum;
            current = current->getNext();
        } while (current != edge.first);

        cycles.push_back(cycle);
        cycleNum++;
    }

    cycleNum = 1;
    auto cycleGraph = Graph<Cycle>();
    auto unboundedFaceCycle = Cycle(true);

    cycleGraph.addNode(cycleNum, unboundedFaceCycle);
    unboundedFaceCycle.setID(cycleNum++);

    dcel->removeUnboundedFace();

    for (auto& cycle: cycles) {
        cycleGraph.addNode(cycleNum, cycle);
        cycle.setID(cycleNum++);
    }
    for (auto& cycle: cycles) {
        if (!cycle.isBoundaryCycle()) {
            auto leftmostVertex = cycle.getLeftmostVertex();
            auto leftEdge = leftmostVertex->getLeftEdge();
            std::vector<Segment> validSegments;
            if (leftEdge.first) validSegments.emplace_back(leftEdge.first);
            if (leftEdge.second) validSegments.emplace_back(leftEdge.second);

            auto it = std::begin(validSegments);
            for (; it != std::end(validSegments); ++it){
                if (approximatelyEqual(it->getSegmentCurrentLongitude(leftmostVertex->getValue().getLatitude()),
                                       leftmostVertex->getValue().getLongitude()))
                    break;
            }

            if(it != std::end(validSegments)){
                auto edge = it->getEdge();
                auto containingCycle = std::find_if(
                        cycles.begin(), cycles.end(), [edge](const Cycle& cycle) {
                            return std::find(cycle.getCycle().begin(), cycle.getCycle().end(), edge) != cycle.getCycle().end();
                        });
                cycleGraph.addEdge(containingCycle->getID(), cycle.getID());
            } else {
                cycleGraph.addEdge(unboundedFaceCycle.getID(), cycle.getID());
            }
        }
    }

    auto connectivity = Connectivity<Cycle>(cycleGraph);
    connectivity.calculateTarjan();
    std::map<int, std::vector<Cycle>> connectedComponents{};

    for (const auto& node: cycleGraph.getNodeSet()) {
        if (connectedComponents.find(node.second->getSCCSID()) == connectedComponents.end()) {
            auto vec = std::vector<Cycle>();
            vec.push_back(node.second->getInfo());
            connectedComponents.insert(std::make_pair(
                    node.second->getSCCSID(), vec)
                    );
        } else connectedComponents.find(node.second->getSCCSID())->second.push_back(node.second->getInfo());
    }

    for (const auto& connectedComponent: connectedComponents) {
        Cycle boundaryCycle;
        std::vector<Cycle> holeCycles;

        for (const auto& cycle: connectedComponent.second) {
            if (cycle.isBoundaryCycle() || cycle.isUnboundedCycle()){
                boundaryCycle = cycle;
            } else holeCycles.push_back(cycle);
        }

        auto face = std::make_shared<Face<GeographicPoint>>();

        if (!boundaryCycle.isUnboundedCycle()) {
            face->setOuter(boundaryCycle.getCycle().front());
            for (const auto& oldFace: boundaryCycle.getFaces()){
                if (!oldFace) continue;
                auto property = oldFace->getProperty();
                face->addProperty(property);
            }
        }

        for (const auto& hole: holeCycles) {
            face->addInner(hole.getCycle().front());
        }

        for (const auto& edge: boundaryCycle.getCycle())
            edge->setIncident(face);

        for (const auto& hole: holeCycles)
            for (const auto& edge: hole.getCycle())
                edge->setIncident(face);

        dcel->addFace(face);
        if (boundaryCycle.isUnboundedCycle())
            dcel->setUnboundedFace(face);
    }
}

