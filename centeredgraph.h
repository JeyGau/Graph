#ifndef NEIGHBOURNODES_H
#define NEIGHBOURNODES_H

#include "boundary.h"

template <typename T> class Graph;

template <typename T>
class CenteredGraph : public Graph<T>
{
public:
    CenteredGraph(T *from, Graph<T> *parent);
    ~CenteredGraph();

    inline T* getOrigin() const { return m_origin; }

    void add(CenteredGraph *graph);

private:
    T* m_origin;

};

template <typename T>
CenteredGraph<T>::CenteredGraph(T *from, Graph<T> *parent)
    : Graph<T> (parent->getBoundariesFromNode(from)),
      m_origin(from)
{

}

template <typename T>
CenteredGraph<T>::~CenteredGraph()
{

}

template <typename T>
void CenteredGraph<T>::add(CenteredGraph *graph)
{
    if(this->getNodes().contains(graph->m_origin)){
        typename QVector<Boundary<T>*>::const_iterator it = graph->m_bounds.constBegin();
        Boundary<T> *bound = this->getDirectBoundary(m_origin, graph->m_origin);

        for (; it != graph->m_bounds.constEnd(); ++it) {
            Boundary<T> *tmpBound = bound->addBound(*it);

            if(tmpBound != Boundary<T>::NULL_BOUNDARY){
                this->m_bounds.append(tmpBound);
                this->addNodesFromBound(*tmpBound);
            }
        }
        this->removeBoundary(bound);
    }
}

#endif // NEIGHBOURNODES_H
