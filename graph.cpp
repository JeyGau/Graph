#ifndef GRAPH_H
#define GRAPH_H

#include <QDebug>
#include <QVector>
#include <QObject>

#include "route.cpp"
#include "centeredgraph.cpp"

template <typename T> class CenteredGraph;
template <typename T> class Graph;

template <typename T>
class Graph
{

public:
    Graph(QVector<Route<T>*> grph);
    ~Graph();

    inline  QVector<Route<T>*> getBounds() { return m_bounds; }
    inline  QVector<T*> getNodes()  { return  m_nodes; }

    CenteredGraph<T>* getCenteredGraph(T* from);

    void removeRoute(Route<T>* bound);

    int getFastestItinerary_Dijkstra(T* src, T* dst);
    int getFastestItineray_Aco(T* src, T* dst);

    QVector<Route<T>*> getBoundariesFromNode(T* from);

private:
    Route<T> *getShortestRoute() ;

    void addReversedBoundaries();

protected:
    Route<T> *getDirectRoute(T* src, T* dst) ;

    inline bool contains(T *node) { return m_bounds.contains(node); }
    inline bool contains(Route<T> *bound) { return m_bounds.contains(bound); }

    void addNodesFromBound(Route<T> &bound);

protected:
    QVector<Route<T>*> m_bounds;
    QVector<T*> m_nodes;
};


template <typename T>
Graph<T>::Graph(QVector<Route<T>*> graph)
    : m_bounds(graph)
{
    typename QVector<Route<T>*>::const_iterator it = graph.constBegin();
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
void Graph<T>::addNodesFromBound(Route<T> &bound)
{
    if(!m_nodes.contains(bound.Source())){
        m_nodes.append(bound.Source());
    }

    if(!m_nodes.contains(bound.Destination())){
        m_nodes.append(bound.Destination());
    }
}

template <typename T>
Route<T> *Graph<T>::getDirectRoute(T* src, T* dst)
{
    typename QVector<Route<T>*>::const_iterator it = m_bounds.constBegin();
    for(; it != m_bounds.constEnd(); ++it){
        Route<T> *tmpBound = (*it);

        if(tmpBound->Source() == src
                && tmpBound->Destination() == dst){
            return tmpBound;
        }
    }
    return nullptr;
}

template <typename T>
Route<T> *Graph<T>::getShortestRoute()
{
    Route<T> *shortestBound = m_bounds.first();

    typename QVector<Route<T>*>::const_iterator it = m_bounds.constBegin();
    for(; it != m_bounds.constEnd(); ++it){
        Route<T> *tmpBound = (*it);

        if(tmpBound->Distance() < shortestBound->Distance()){
            shortestBound = tmpBound;
        }
    }
    return shortestBound;
}

template <typename T>
int Graph<T>::getFastestItinerary_Dijkstra(T* src, T* dst)
{
    CenteredGraph<T> *tmpGraph = this->getCenteredGraph(src);
    Route<T> *shortestBound;
    int it = 0;

    do{
        qDebug() << "iteration " << ++it;
        shortestBound = tmpGraph->getShortestRoute();

        CenteredGraph<T> *graph = new CenteredGraph<T>(shortestBound->Destination(), this);
        tmpGraph->add(graph);

        delete graph;

    }while(shortestBound->Destination() != dst);

    int distance = shortestBound->Distance();

    delete tmpGraph;

    return distance;
}

template <typename T>
int Graph<T>::getFastestItineray_Aco(T* src, T* dest)
{
    Q_UNUSED(src)
    Q_UNUSED(dest)
    return 0;
}

template <typename T>
CenteredGraph<T>* Graph<T>::getCenteredGraph(T* from)
{
    return new CenteredGraph<T>(from, this);
}

template <typename T>
void Graph<T>::removeRoute(Route<T>* bound)
{
    if(m_bounds.contains(bound))
        m_bounds.remove(m_bounds.indexOf(bound));
}

template <typename T>
QVector<Route<T>*> Graph<T>::getBoundariesFromNode(T* from)
{
    QVector<Route<T>*> boundaries;

    typename QVector<Route<T>*>::const_iterator it = m_bounds.constBegin();
    for(; it != m_bounds.constEnd(); ++it){
        if(static_cast<Route<T>*>(*it)->Source() == from){
            boundaries.append(new Route<T>(**it));
        }
    }
    return boundaries;
}

template <typename T>
void Graph<T>::addReversedBoundaries()
{
    QVector<Route<T>*> tmpGraph = m_bounds;
    typename QVector<Route<T>*>::const_iterator it = tmpGraph.constBegin();
    for(; it != tmpGraph.constEnd(); ++it){
        Route<T> *tmpBound = (*it);
        if(tmpBound->getReversedRoute() != Route<T>::NULL_Route){
            m_bounds.append(tmpBound->getReversedRoute());
        }
    }
}


#endif // GRAPH_H
