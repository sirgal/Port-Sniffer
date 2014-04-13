#ifndef CHANNELFACTORY_H
#define CHANNELFACTORY_H

#include <QList>

#include "portdatasorter.h"
#include "channels/channel.h"
#include "ports/port.h"
#include "ports/portfactory.h"

using ChannelPointer = std::shared_ptr<Channel>;

class ChannelFactory
{
    PortFactory port_factory;
    PortDataSorter *data_sorter;
    QList<ChannelPointer> channel_list;

public:
    ChannelFactory( PortDataSorter *data_sorter ) :
        data_sorter(data_sorter)
    { }

    void startAll()
    {
        data_sorter->start();

        foreach( Channel ch, channel_list ) {
            try {
                ch.start();
            } catch( QString &error_string ) {
                stopAll();
                throw;
            }
        }
    }

    void stopAll()
    {
        foreach( Channel ch, channel_list )
            ch.stop();
    }

    ChannelPointer addChannel( ChannelSettings &settings, PortSettings &port_settings )
    {
        PortPointer port = port_factory.buildPort( port_settings );

        ChannelPointer new_channel = std::make_shared<Channel>( port, settings );
        channel_list.append( new_channel );

        connect( new_channel.get(), SIGNAL(gotByte(char)), data_sorter, SLOT(byteReceived(char)) );
    }

    void removeChannel( ChannelPointer channel )
    {
        channel->stop();
        channel_list.removeAll( channel );

        disconnect( channel.get(), SIGNAL(gotByte(char)), data_sorter, SLOT(byteReceived(char)) );
    }

    ChannelPointer findChannel( int number )
    {
        foreach( ChannelPointer ch, channel_list ) {
            if( ch->getSettings().getNumber() == number )
                return ch;
        }

        return nullptr;
    }
};

#endif // CHANNELFACTORY_H
