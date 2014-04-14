#include "channels/channel_factory.h"

void ChannelFactory::startAll()
{
    data_sorter->start();

    foreach( ChannelPointer ch, channel_list ) {
        try {
            ch->start();
        } catch( QString &error_string ) {
            stopAll();
            throw;
        }
    }
}

void ChannelFactory::stopAll()
{
    foreach( ChannelPointer ch, channel_list )
        ch->stop();
}

ChannelPointer ChannelFactory::addChannel(ChannelSettings &settings, PortSettings &port_settings)
{
    PortPointer port = port_factory.buildPort( port_settings );

    ChannelPointer new_channel = std::make_shared<Channel>( port, settings );
    channel_list.append( new_channel );

    connect( new_channel.get(), SIGNAL(gotByte(char)), data_sorter, SLOT(byteReceived(char)) );
}

void ChannelFactory::removeChannel(ChannelPointer channel)
{
    channel->stop();
    channel_list.removeAll( channel );

    disconnect( channel.get(), SIGNAL(gotByte(char)), data_sorter, SLOT(byteReceived(char)) );
}

ChannelPointer ChannelFactory::findChannel(int number)
{
    foreach( ChannelPointer ch, channel_list ) {
        if( ch->getSettings().getNumber() == number )
            return ch;
    }

    return nullptr;
}
