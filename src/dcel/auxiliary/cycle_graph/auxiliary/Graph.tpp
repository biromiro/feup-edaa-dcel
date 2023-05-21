//
// Created by nrtc on 03/05/2021.
//

#ifndef FEUP_CAL_PROJ_GRAPH_TPP
#define FEUP_CAL_PROJ_GRAPH_TPP

#include "Node.tpp"
#include "Edge.tpp"
#include <vector>
#include <map>
#include <queue>
#include <iostream>

/* ================================================================================================
 * Class Graph
 * ================================================================================================
 */

template <class T>
class Graph {
    std::map<int, Node<T>*> nodeSet;

public:
    void freeGraph();
    Node<T>* findNode(int id) const;
    std::map<int, Node<T>*> getNodeSet() const;
    Node<T> *addNode(int ID, const T &in);
    virtual Edge<T> *addEdge(int srcID, int destID);
};

template <class T>
Node<T> * Graph<T>::addNode(int ID, const T &in) {
    Node<T> *v = findNode(ID);
    if (v != nullptr)
        return v;
    v = new Node<T>(ID, in);
    nodeSet[ID] = v;
    return v;
}

template <class T>
std::map<int, Node<T>*> Graph<T>::getNodeSet() const {
    return nodeSet;
}

template<class T>
Node<T> *Graph<T>::findNode(int id) const {
    auto node = nodeSet.find(id);
    if(node != nodeSet.end()) return node->second;
    return nullptr;
}

template<class T>
Edge<T> *Graph<T>::addEdge(int srcID, int destID) {
    auto s = this->findNode(srcID);
    auto d = this->findNode(destID);
    if (s == nullptr || d == nullptr)
        return nullptr;
    auto *directed = new Edge<T>(s, d);
    auto *reversed = new Edge<T>(d, s);
    directed->reverse = reversed;
    reversed->reverse = directed;

    s->addEdge(directed);
    d->addEdge(reversed);

    return directed;
}

template<class T>
void Graph<T>::freeGraph() {
    for(auto& node : nodeSet) {
        std::vector<Edge<T>*> out = node.second->getOutgoing();
        for(Edge<T>* outgoing : out) {
            delete outgoing;
        }
        delete node.second;
    }
}
#endif //FEUP_CAL_PROJ_GRAPH_TPP