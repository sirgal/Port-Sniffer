#include "channels/channel_factory.h"

PortSettingsPointer ChannelFactory::createDummyPort()
{
    return std::dynamic_pointer_cast<PortSettings>( std::make_shared<DummyPortSettings>( ) );
}

ChannelPointer ChannelFactory::findChannel( int chan_num )
{
    foreach( ChannelPointer ch, channel_list ) {
        if( ch->getSettings().getNumber() == chan_num )
            return ch;
    }

    return nullptr;
}

void ChannelFactory::storeSettings( PortSettingsPointer settings, int chan_num )
{
    ChannelPointer channel = findChannel( chan_num );

    if( channel )
        channel->setPortSettings( settings );
}

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

ChannelPointer ChannelFactory::addChannel( int chan_num )
{
    PortSettingsPointer port_settings = createDummyPort();
    ChannelSettings channel_settings( chan_num );

    ChannelPointer new_channel = std::make_shared<Channel>( channel_settings );
    new_channel->setPortSettings( port_settings );
    channel_list.append( new_channel );

    connect( new_channel.get(), SIGNAL(gotByte(char)), data_sorter, SLOT(byteReceived(char)) );

    return new_channel;
}

void ChannelFactory::removeChannel(int chan_num)
{
    ChannelPointer channel = findChannel( chan_num );

    if( channel ) {
        channel->stop();
        channel_list.removeAll( channel );

        disconnect( channel.get(), SIGNAL(gotByte(char)), data_sorter, SLOT(byteReceived(char)) );
    }
}

PortSettingsPointer ChannelFactory::getPortSettings( int chan_num )
{
    ChannelPointer channel = findChannel( chan_num );

    if( channel )
        return channel->getPortSettings();
    else
        return nullptr;
}

QColor ChannelFactory::getChannelColor(int chan_num)
{
    ChannelPointer channel = findChannel( chan_num );

    if( channel )
        return channel->getSettings().getColor();
    else
        return Qt::black;
}

void ChannelFactory::setChannelColor(int chan_num, QColor color)
{
    ChannelPointer channel = findChannel( chan_num );

    if( channel )
        channel->getSettings().setColor( color );
}

bool ChannelFactory::isChannelEnabled(int chan_num)
{
    ChannelPointer channel = findChannel( chan_num );

    if( channel )
        return channel->getSettings().isEnabled();
    else
        return false;
}

void ChannelFactory::setEnabledChannel( int chan_num, bool is_enabled )
{
    ChannelPointer channel = findChannel( chan_num );

    if( channel )
        channel->getSettings().setEnabled( is_enabled );
}
