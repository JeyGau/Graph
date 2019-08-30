#ifndef Route_H
#define Route_H

#include <QtMath>
#include <QDebug>
#include <QString>

template <typename T> class Route;

template <typename T>
class Route
{
public:
    static  Route<T>* NULL_Route;

    Route(T *src, T *dest, unsigned int dist, Route<T>* reversed = NULL_Route, float phrmn = 0);
    Route(const Route &copy);

    ~Route();

    inline T *Source()  { return m_src; }
    inline T *Destination()  { return m_dest; }

    inline unsigned int Distance()  { return m_dist; }
    inline unsigned int Pheromone() { return m_pheromone; }

    QString toString() ;
    Route<T> *getReversedRoute();

    Route<T>* addBound(Route<T>* bound);

    inline Route<T>* operator-() { return getReversedRoute(); }

private:
    Route();

private:
    T *m_src;
    T *m_dest;
    unsigned int m_dist;
    float m_pheromone;
    Route<T>* m_reversedBound;

};

template <typename T>
Route<T>* Route<T>::NULL_Route = new Route<T>();

template <typename T>
Route<T>::Route()
    : m_src(nullptr),
      m_dest(nullptr),
      m_dist(0),
      m_pheromone(0),
      m_reversedBound(nullptr)
{
}

template <typename T>
Route<T>::Route(T* src,
                T* dest,
                unsigned int dist,
                Route<T>* reversed,
                float phrmn)
    : m_src(src),
      m_dest(dest),
      m_dist(dist),
      m_pheromone(phrmn),
      m_reversedBound(reversed)
{
}

template <typename T>
Route<T>::Route(const Route<T> &copy)
{
    m_src = copy.m_src;
    m_dest = copy.m_dest;

    m_reversedBound = NULL_Route;
    m_dist = copy.m_dist;
    m_pheromone = copy.m_pheromone;
}

template <typename T>
Route<T>::~Route<T>()
{
}

template <typename T>
QString Route<T>::toString()
{
    return "Route from pointer " + Source()
            + " to pointer " + Destination()
            + " with a length of " + QString::number(Distance())
            + " and a pheromon level of " + QString::number(m_pheromone);
}

template <typename T>
Route<T> * Route<T>::getReversedRoute()
{
    if(m_reversedBound == nullptr)
        m_reversedBound =  new Route<T>(m_dest,
                                        m_src,
                                        m_dist,
                                        this,
                                        m_pheromone);

    return m_reversedBound;
}

template <typename T>
Route<T>* Route<T>::addBound(Route<T>* bound)
{
    if(bound == nullptr
            || bound == NULL_Route)
        return NULL_Route;

    else if(bound->Destination() == Source())
        return NULL_Route;

    else if(Destination() == bound->Source())
        return new Route<T>(m_src,
                            bound->Destination(),
                            m_dist + bound->Distance(),
                            NULL_Route,
                            m_pheromone);

    else
        return NULL_Route;
}

#endif // Route_H
