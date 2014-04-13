#ifndef PORTDATASORTER_H
#define PORTDATASORTER_H

#include <QList>
#include <QPair>
#include <QObject>
#include <QElapsedTimer>

#include "ports/port.h"
#include "channels/channel.h"

using channel_data_pair = QPair<int, char>;
using timestamped_data = QPair<channel_data_pair, long long int>;

// receives byte from some port, puts it into pair, sends to paramstringprocessor
class PortDataSorter : public QObject
{
    Q_OBJECT

    QElapsedTimer timer;

public slots:
    void byteReceived( char byte );

signals:
    void gotByte( timestamped_data byte );

public:
    PortDataSorter() { }
    ~PortDataSorter();

    void start()
    {
        timer.start();
    }
};

#endif // PORTDATASORTER_H
