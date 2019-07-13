#ifndef BOUNDARY_H
#define BOUNDARY_H

#include <QtMath>
#include <QDebug>
#include <QString>

template <typename T> class Boundary;

template <typename T>
class Boundary
{
public:
    static  Boundary<T>* NULL_BOUNDARY;

    Boundary(T *src, T *dest, unsigned int dist, Boundary<T>* reversed = NULL_BOUNDARY);
    Boundary(const Boundary &copy);

    ~Boundary();

    inline T *Source()  { return m_src; }
    inline T *Destination()  { return m_dest; }

    inline unsigned int Distance()  { return m_dist; }

    QString toString() ;
    Boundary<T> *getReversedBoundary();

    Boundary<T>* addBound(Boundary<T>* bound);

    inline Boundary<T>* operator-() { return getReversedBoundary(); }

private:
    Boundary();

private:
    T *m_src;
    T *m_dest;
    unsigned int m_dist;
    Boundary<T>* m_reversedBound;

};

template <typename T>
Boundary<T>* Boundary<T>::NULL_BOUNDARY = new Boundary<T>();

template <typename T>
Boundary<T>::Boundary()
    : m_src(nullptr),
      m_dest(nullptr),
      m_dist(0),
      m_reversedBound(nullptr)
{
}

template <typename T>
Boundary<T>::Boundary(T* src,
                      T* dest,
                      unsigned int dist,
                      Boundary<T>* reversed)
    : m_src(src),
      m_dest(dest),
      m_dist(dist),
      m_reversedBound(reversed)
{
}

template <typename T>
Boundary<T>::Boundary(const Boundary<T> &copy)
{
    m_src = copy.m_src;
    m_dest = copy.m_dest;

    m_reversedBound = NULL_BOUNDARY;
    m_dist = copy.m_dist;
}

template <typename T>
Boundary<T>::~Boundary<T>()
{
}

template <typename T>
QString Boundary<T>::toString()
{
    return "Boundary from pointer " + Source()
            + " to pointer " + Destination()
            + " with a length of " + QString::number(Distance());
}

template <typename T>
Boundary<T> * Boundary<T>::getReversedBoundary()
{
    if(m_reversedBound == nullptr)
        m_reversedBound =  new Boundary<T>(m_dest, m_src, m_dist, this);

    return m_reversedBound;
}

template <typename T>
Boundary<T>* Boundary<T>::addBound(Boundary<T>* bound)
{    
    if(bound == nullptr
            || bound == NULL_BOUNDARY)
        return NULL_BOUNDARY;

    else if(bound->Destination() == Source())
        return NULL_BOUNDARY;

    else if(Destination() == bound->Source())
        return new Boundary<T>(m_src,
                               bound->Destination(),
                               m_dist + bound->Distance());

    else
        return NULL_BOUNDARY;
}

#endif // BOUNDARY_H
