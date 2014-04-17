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

PortPointer Channel::getPort()
{
    return port;
}

void Channel::setPortSettings( PortSettingsPointer settings )
{
    port = settings->buildAccordingPort();
}

void Channel::start()
{
    try {
        port->enable();
        connect( port.get(), SIGNAL(gotByte(char)), this, SLOT(byteReceived(char)) );
    } catch( QString &error_string ) {
        throw;
    }
}

void Channel::stop()
{
    port->disable();
    disconnect( port.get(), SIGNAL(gotByte(char)), this, SLOT(byteReceived(char)) );
}
