
#ifndef NEIGHBOURNODES_H
#define NEIGHBOURNODES_H

#include "route.cpp"

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
        typename QVector<Route<T>*>::const_iterator it = graph->m_bounds.constBegin();
        Route<T> *bound = this->getDirectRoute(m_origin, graph->m_origin);

        for (; it != graph->m_bounds.constEnd(); ++it) {
            Route<T> *tmpBound = bound->addBound(*it);

            if(tmpBound != Route<T>::NULL_Route){
                this->m_bounds.append(tmpBound);
                this->addNodesFromBound(*tmpBound);
            }
        }
        this->removeRoute(bound);
    }
}

#endif // NEIGHBOURNODES_H
