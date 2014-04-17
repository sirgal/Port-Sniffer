#ifndef CHANNELFACTORY_H
#define CHANNELFACTORY_H

#include <QObject>
#include <QList>

#include "portdatasorter.h"
#include "channels/channel.h"
#include "ports/port.h"
#include "ports/port_factory.h"
#include "ports/dummyport.h"

using ChannelPointer = std::shared_ptr<Channel>;

class ChannelFactory : public QObject
{
    Q_OBJECT

    PortFactory port_factory;
    PortDataSorter *data_sorter;
    QList<ChannelPointer> channel_list;

public:
    ChannelFactory( PortDataSorter *data_sorter ) :
        data_sorter(data_sorter)
    { }

    void startAll();
    void stopAll();

    ChannelPointer addChannel( int number );
    ChannelPointer addChannel( int number, PortSettings &port_settings );
    void removeChannel( ChannelPointer channel );
    ChannelPointer findChannel( int number );
};

#endif // CHANNELFACTORY_H
