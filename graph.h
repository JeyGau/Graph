#ifndef GRAPH_H
#define GRAPH_H

#include <QDebug>
#include <QObject>

#include "boundary.h"

template <typename T> class Graph;

template <typename T>
class Graph : public QObject
{
    Q_OBJECT

public:
    Graph(const QVector<Boundary<T>*> &grph, QObject* parent = nullptr);

    inline const QVector<Boundary<T>*>& getBounds() const { return m_bounds; }
    inline const QVector<T*>& getNodes() const { return  m_nodes; }

    const Boundary<T> *getDirectBoundary(T* src, T* dst) const;

    void removeBoundary(Boundary<T>* bound);
    int getShortestItinerary_Dijkstra(T* src, T* dst) const;

    QVector<Boundary<T>*> getBoundariesFromNode(T* from);

    inline const Boundary<T>* operator-(Boundary<T>* bound) const { return bound->getReversedBoundary(); }

private:
    const Boundary<T> *getShortestBoundary() const;
    bool contains(T *node);

    void addReversedBoundaries();

private:
    QVector<Boundary<T>*> m_bounds;
    QVector<T*> m_nodes;
};

#include <QVector>

template <typename T>
Graph<T>::Graph(const QVector<Boundary<T>*> &graph, QObject* parent)
    : QObject(parent),
      m_bounds(graph)
{
    typename QVector<Boundary<T>*>::const_iterator it = m_bounds.constBegin();
    for(; it != m_bounds.constEnd(); ++it){
        Boundary<T> *tmpBound = (*it);

        if(!m_nodes.contains(tmpBound->Source())){
            m_nodes.append(tmpBound->Source());
            qDebug() << "adding new node from source";
        }

        if(!m_nodes.contains(tmpBound->Destination())){
            m_nodes.append(tmpBound->Destination());
            qDebug() << "adding new node from destination";
        }
    }

    addReversedBoundaries();
}

template <typename T>
const Boundary<T> *Graph<T>::getDirectBoundary(T* src, T* dst) const
{
    typename QVector<Boundary<T>*>::const_iterator it = m_bounds.constBegin();
    for(; it != m_bounds.constEnd(); ++it){
        Boundary<T> *tmpBound = (*it);

        qDebug() << (tmpBound == nullptr);

        if(tmpBound->Source() == src
                && tmpBound->Destination() == dst){
            return tmpBound;
        }
    }
    return nullptr;
}

template <typename T>
const Boundary<T> *Graph<T>::getShortestBoundary() const
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
int Graph<T>::getShortestItinerary_Dijkstra(T* src, T* dst) const
{
    Graph<T> *tmpGraph = new Graph(getBoundariesFromNode(src));

    while(!tmpGraph->contains(dst)){

    }

}

template <typename T>
void Graph<T>::removeBoundary(Boundary<T>* bound)
{
    if(contains(bound))
        m_bounds.remove(m_bounds.indexOf(bound));
}

template <typename T>
QVector<Boundary<T>*> Graph<T>::getBoundariesFromNode(T* from)
{
    QVector<Boundary<T>*> boundaries;

    typename QVector<Boundary<T>*>::const_iterator it = m_bounds.constBegin();
    for(; it != m_bounds.constEnd(); ++it){
        Boundary<T> *tmpBound = (*it);

        if(tmpBound->Source() == from){
            boundaries.append(tmpBound);
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
    qDebug() << "reverse boundaries added";
}

template <typename T>
bool Graph<T>::contains(T *node)
{
    return m_bounds.contains(node);
}


#endif // GRAPH_H
