#ifndef LIMITEDSIZEPAIRLIST_H
#define LIMITEDSIZEPAIRLIST_H

#include <QList>
#include <QPair>
#include <QDebug>

template <class first, class second>
class limitedSizePairList
{
    QList< QPair<first, second> > *pairs;
    int maxSize;

    void fixSize()
    {
        pairs->removeFirst();
    }

public:
    limitedSizePairList(int max_size )
        : maxSize(max_size)
    {
        pairs = new QList<QPair<first, second> >;
    }

    ~limitedSizePairList()
    {
        delete pairs;
    }

    QList< QPair<first, second> > *returnElements()
    {
        return pairs;
    }

    void append( QPair<first, second> pair )
    {
        pairs->append( pair );
        if( pairs->size() > maxSize )
            fixSize();
    }

    void append( first a, second b )
    {
        append( QPair<first, second>( a, b ) );
    }

    void clear()
    {
        pairs->clear();
    }

    int size()
    {
        return pairs->size();
    }

    QPair<first, second> &operator[]( int i ) {
        return pairs->at(i);
    }

    QPair<first, second> at( int i ) {
        return pairs->at(i);
    }

    int length()
    {
        return pairs->length();
    }
};

#endif // LIMITEDSIZEPAIRLIST_H
