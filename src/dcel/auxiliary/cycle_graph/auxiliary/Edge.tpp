//
// Created by nrtc on 03/05/2021.
//

#ifndef FEUP_CAL_PROJ_EDGE_TPP
#define FEUP_CAL_PROJ_EDGE_TPP

template<class T>
class Node;

template<class T>
class Graph;

/* ================================================================================================
 * Class Edge
 * ================================================================================================
 */

template<class T>
class Edge {
    Node<T> *orig;
    Node<T> *dest;
    Edge<T> *reverse;
    bool selected;

    Edge(Node<T> *o, Node<T> *d);

public:
    Node<T> *getOrig() const;

    Node<T> *getDest() const;

    Edge<T>* getReverse() const;

    void select();

    void unSelect();

    bool isSelected() const;

    friend class Graph<T>;

    friend class Node<T>;
};

template<class T>
Edge<T>::Edge(Node<T> *o, Node<T> *d):
        orig(o), dest(d), selected(false), reverse(nullptr) {}

template<class T>
Node<T> *Edge<T>::getOrig() const {
    return orig;
}


template<class T>
Node<T> *Edge<T>::getDest() const {
    return dest;
}

#endif //FEUP_CAL_PROJ_EDGE_TPP