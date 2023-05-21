#ifndef FEUP_CAL_PROJ_NODE_TPP
#define FEUP_CAL_PROJ_NODE_TPP

#include "Edge.tpp"
#include <limits>
#include "vector"

template <class T> class Graph;

constexpr auto INF = std::numeric_limits<double>::max();

/*
 * ================================================================================================
 * Class Node
 * ================================================================================================
 */

template <class T>
class Node {
    T info;
    std::vector<Edge<T> *> outgoing;
    std::vector<Edge<T> *> incoming;
    std::vector<Edge<T> *> walking;
    bool visited = false;
    Node<T> *path;
    double dist = INF;
    int ID = 0;
    int sccsID = 0;
    int queueIndex = 0;
    int lowlink = 0;
    bool inStack = false;
    void addEdge(Edge<T> *e);
    bool operator<(Node<T> & node) const;
    Node(int id, T in);

public:
    T getInfo() const;
    std::vector<Edge<T> *> getIncoming() const;
    std::vector<Edge<T> *> getOutgoing() const;
    void setVisited(bool visited);
    void setPath(Node<T> *path);
    void setDist(double dist);
    [[nodiscard]] bool isVisited() const;
    Node<T> *getPath();
    [[nodiscard]] double getDist() const;
    void setID(int newID);
    [[nodiscard]] int getID() const;
    [[nodiscard]] int getSCCSID() const;
    void setSCCSID(int sccsId);
    [[nodiscard]] int getLowlink() const;
    void setLowlink(int lowlink);
    [[nodiscard]] bool isInStack() const;
    void setInStack(bool inStack);

    friend class Graph<T>;

    void addWalkingEdge(Edge<T> *e1, Edge<T> *e2);

    const std::vector<Edge<T> *> &getWalking() const;
};


template<class T>
void Edge<T>::select() {
    this->selected = true;
}

template<class T>
bool Edge<T>::isSelected() const {
    return this->selected;
}

template<class T>
Edge<T> *Edge<T>::getReverse() const {
    return reverse;
}

template<class T>
void Edge<T>::unSelect() {
    this->selected = false;
}


template <class T>
Node<T>::Node(int id, T in): ID(id), info(in) {}

template <class T>
void Node<T>::addEdge(Edge<T> *e) {
    outgoing.push_back(e);
    e->dest->incoming.push_back(e);
}

template<class T>
const std::vector<Edge<T> *> &Node<T>::getWalking() const {
    return walking;
}

template <class T>
void Node<T>::addWalkingEdge(Edge<T> *e1, Edge<T>*e2) {
    walking.push_back(e1);

    e2->orig->walking.push_back(e2);
}

template <class T>
bool Node<T>::operator<(Node<T> & node) const {
    return this->dist < node.dist;
}

template <class T>
T Node<T>::getInfo() const {
    return this->info;
}

template <class T>
std::vector<Edge<T> *>  Node<T>::getIncoming() const {
    return this->incoming;
}

template <class T>
std::vector<Edge<T> *>  Node<T>::getOutgoing() const {
    return this->outgoing;
}

template<class T>
void Node<T>::setVisited(bool newVisited) {
    Node::visited = newVisited;
}

template<class T>
void Node<T>::setPath(Node<T> *newPath) {
    Node::path = newPath;
}

template<class T>
void Node<T>::setDist(double newDist) {
    Node::dist = newDist;
}

template<class T>
bool Node<T>::isVisited() const {
    return visited;
}

template<class T>
Node<T> *Node<T>::getPath() {
    return path;
}

template<class T>
double Node<T>::getDist() const {
    return dist;
}

template<class T>
void Node<T>::setID(int newID) {
    Node::ID = newID;
}

template<class T>
int Node<T>::getID() const {
    return ID;
}

template<class T>
int Node<T>::getLowlink() const {
    return lowlink;
}

template<class T>
void Node<T>::setLowlink(int lowlink) {
    Node::lowlink = lowlink;
}

template<class T>
bool Node<T>::isInStack() const {
    return inStack;
}

template<class T>
void Node<T>::setInStack(bool inStack) {
    Node::inStack = inStack;
}

template<class T>
int Node<T>::getSCCSID() const {
    return sccsID;
}

template<class T>
void Node<T>::setSCCSID(int sccsId) {
    sccsID = sccsId;
}

template<class T>
struct CmpNodePtrs {
    bool operator()(const Node<T> * lhs, const Node<T> * rhs) const {
        return lhs->getDist() > rhs->getDist();
    }
};


#endif //FEUP_CAL_PROJ_NODE_TPP