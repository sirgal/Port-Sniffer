#include "channels/channel.h"


void Channel::byteReceived(char byte)
{
    emit gotByte(byte);
}

ChannelSettings &Channel::getSettings()
{
    return settings;
}

void Channel::setSettings(const ChannelSettings &value)
{
    settings = value;
}

PortSettingsPointer Channel::getPortSettings()
{
    return port_settings;
}

void Channel::setPortSettings( PortSettingsPointer settings )
{
    port_settings = settings;
    active_port = port_settings->buildAccordingPort();
}

void Channel::start()
{
    if( !active_port )
        return;

    try {
        active_port->enable();
        connect( active_port.get(), SIGNAL(gotByte(char)), this, SLOT(byteReceived(char)) );
    } catch( QString &error_string ) {
        throw;
    }
}

void Channel::stop()
{
    if( !active_port )
        return;

    active_port->disable();
    disconnect( active_port.get(), SIGNAL(gotByte(char)), this, SLOT(byteReceived(char)) );
}
