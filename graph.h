#ifndef GRAPH_H
#define GRAPH_H

#include <QDebug>
#include <QVector>
#include <QObject>

#include "boundary.h"
#include "centeredgraph.h"

template <typename T> class CenteredGraph;
template <typename T> class Graph;

template <typename T>
class Graph
{

public:
    Graph(QVector<Boundary<T>*> grph);
    ~Graph();

    inline  QVector<Boundary<T>*> getBounds() { return m_bounds; }
    inline  QVector<T*> getNodes()  { return  m_nodes; }

    CenteredGraph<T>* getCenteredGraph(T* from);

    void removeBoundary(Boundary<T>* bound);

    int getShortestItinerary_Dijkstra(T* src, T* dst);

    QVector<Boundary<T>*> getBoundariesFromNode(T* from);

private:
    Boundary<T> *getShortestBoundary() ;

    void addReversedBoundaries();

protected:
    Boundary<T> *getDirectBoundary(T* src, T* dst) ;

    inline bool contains(T *node) { return m_bounds.contains(node); }
    inline bool contains(Boundary<T> *bound) { return m_bounds.contains(bound); }

    void addNodesFromBound(Boundary<T> &bound);

protected:
    QVector<Boundary<T>*> m_bounds;
    QVector<T*> m_nodes;
};


template <typename T>
Graph<T>::Graph(QVector<Boundary<T>*> graph)
    : m_bounds(graph)
{
    typename QVector<Boundary<T>*>::const_iterator it = graph.constBegin();
    for(; it != graph.constEnd(); ++it){
        addNodesFromBound(**it);
    }

    addReversedBoundaries();
}

template <typename T>
Graph<T>::~Graph<T>()
{
    while(m_bounds.size()!=0){
        delete m_bounds.takeLast();
    }

    while(m_nodes.size()!=0){
        delete m_nodes.takeLast();
    }

    m_nodes.clear();
    m_bounds.clear();
}

template <typename T>
void Graph<T>::addNodesFromBound(Boundary<T> &bound)
{
    if(!m_nodes.contains(bound.Source())){
        m_nodes.append(bound.Source());
    }

    if(!m_nodes.contains(bound.Destination())){
        m_nodes.append(bound.Destination());
    }
}

template <typename T>
Boundary<T> *Graph<T>::getDirectBoundary(T* src, T* dst)
{
    typename QVector<Boundary<T>*>::const_iterator it = m_bounds.constBegin();
    for(; it != m_bounds.constEnd(); ++it){
        Boundary<T> *tmpBound = (*it);

        if(tmpBound->Source() == src
                && tmpBound->Destination() == dst){
            return tmpBound;
        }
    }
    return nullptr;
}

template <typename T>
Boundary<T> *Graph<T>::getShortestBoundary()
{
    Boundary<T> *shortestBound = m_bounds.first();

    typename QVector<Boundary<T>*>::const_iterator it = m_bounds.constBegin();
    for(; it != m_bounds.constEnd(); ++it){
        Boundary<T> *tmpBound = (*it);

        if(tmpBound->Distance() < shortestBound->Distance()){
            shortestBound = tmpBound;
        }
    }
    return shortestBound;
}

template <typename T>
int Graph<T>::getShortestItinerary_Dijkstra(T* src, T* dst)
{
    CenteredGraph<T> *tmpGraph = this->getCenteredGraph(src);
    Boundary<T> *shortestBound;
    int it = 0;

    do{
        qDebug() << "iteration " << ++it;
        shortestBound = tmpGraph->getShortestBoundary();        

        CenteredGraph<T> *graph = new CenteredGraph<T>(shortestBound->Destination(), this);
        tmpGraph->add(graph);

        delete graph;

    }while(shortestBound->Destination() != dst);

    int distance = shortestBound->Distance();

    delete tmpGraph;

    return distance;
}

template <typename T>
CenteredGraph<T>* Graph<T>::getCenteredGraph(T* from)
{
    return new CenteredGraph<T>(from, this);
}

template <typename T>
void Graph<T>::removeBoundary(Boundary<T>* bound)
{
    if(m_bounds.contains(bound))
        m_bounds.remove(m_bounds.indexOf(bound));
}

template <typename T>
QVector<Boundary<T>*> Graph<T>::getBoundariesFromNode(T* from)
{
    QVector<Boundary<T>*> boundaries;

    typename QVector<Boundary<T>*>::const_iterator it = m_bounds.constBegin();
    for(; it != m_bounds.constEnd(); ++it){
        if(static_cast<Boundary<T>*>(*it)->Source() == from){
            boundaries.append(new Boundary<T>(**it));
        }
    }
    return boundaries;
}

template <typename T>
void Graph<T>::addReversedBoundaries()
{
    QVector<Boundary<T>*> tmpGraph = m_bounds;
    typename QVector<Boundary<T>*>::const_iterator it = tmpGraph.constBegin();
    for(; it != tmpGraph.constEnd(); ++it){
        Boundary<T> *tmpBound = (*it);
        if(tmpBound->getReversedBoundary() != Boundary<T>::NULL_BOUNDARY){
            m_bounds.append(tmpBound->getReversedBoundary());
        }
    }
}


#endif // GRAPH_H
