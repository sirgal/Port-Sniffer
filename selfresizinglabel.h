#ifndef SELFRESIZINGLABEL_H
#define SELFRESIZINGLABEL_H

#include <QLabel>
#include <QResizeEvent>

class SelfResizingLabel : public QLabel
{
    Q_OBJECT
signals:
    void newSize( QSize size );
public:
    SelfResizingLabel(QWidget *parent ) : QLabel( parent ) { }

    void resizeEvent(QResizeEvent *event)
    {
        setMinimumSize( 1, 1 );
        emit newSize( event->size() );
    }
};

#endif // SELFRESIZINGLABEL_H
