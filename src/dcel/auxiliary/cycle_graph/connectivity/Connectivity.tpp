//
// Created by nrtc on 05/05/2021.
//

#ifndef FEUP_CAL_PROJ_CONNECTIVITY_TPP
#define FEUP_CAL_PROJ_CONNECTIVITY_TPP

#include "../auxiliary/Graph.tpp"
#include <stack>

template <class T>
class Connectivity {
public:
    explicit Connectivity(Graph<T>& graph);
    bool isConnected();
    int getNumConnectedComponents();
private:
    int sccs = -1;
    Graph<T> currentGraph;
    bool calculateTarjan();
    void dfs(int& currentID, Node<T> *node, std::stack<Node<T>*>& stack);
};

template<class T>
Connectivity<T>::Connectivity(Graph<T> &graph) {
    this->currentGraph = graph;
}

template<class T>
bool Connectivity<T>::isConnected() {
    return calculateTarjan();
}

template<class T>
int Connectivity<T>::getNumConnectedComponents() {
    if(sccs == -1) calculateTarjan();
    return sccs;
}

template<class T>
bool Connectivity<T>::calculateTarjan() {
    sccs = 0;
    int currentID = 0;
    std::stack<Node<T>*> stack = std::stack<Node<T>*>();

    for(auto pair: currentGraph.getNodeSet())
        pair.second->setVisited(false);

    for(auto pair: currentGraph.getNodeSet())
        if(!(pair.second->isVisited())) dfs(currentID, pair.second, stack);

    return sccs == 1;
}

template<class T>
void Connectivity<T>::dfs(int &currentID, Node<T> *node, std::stack<Node<T>*> &stack) {
    stack.push(node);
    node->setVisited(true);
    node->setInStack(true);
    node->setSCCSID(currentID);
    node->setLowlink(currentID);
    currentID++;

    for(Edge<T>* adjacent: node->getOutgoing()){
        Node<T>* adj = adjacent->getDest();
        if(!(adj->isVisited())) dfs(currentID, adj, stack);
        if(adj->isInStack()) node->setLowlink(std::min(adj->getLowlink(), node->getLowlink()));
    }

    if(node->getSCCSID() == node->getLowlink()){
        Node<T> * cur;
        while((cur = stack.top()) != node){
            cur->setSCCSID(node->getSCCSID());
            cur->setInStack(false);
            stack.pop();
        }
        cur->setInStack(false);
        stack.pop();
        sccs++;
    }
}

#endif //FEUP_CAL_PROJ_CONNECTIVITY_TPP