#include "channels/channel_settings.h"

void ChannelSettings::setColor(QColor &new_port_color)
{
    port_color = new_port_color;
}


QString ChannelSettings::getPortTypeName()
{
    return port_type_name;
}

void ChannelSettings::setPortTypeName(QString &value)
{
    port_type_name = value;
}

void ChannelSettings::enable()
{
    is_enabled = true;
}

void ChannelSettings::disable()
{
    is_enabled = false;
}

QColor ChannelSettings::getColor()
{
    return port_color;
}

int ChannelSettings::getNumber()
{
    return channel_number;
}

bool ChannelSettings::isEnabled()
{
    return is_enabled;
}

bool ChannelSettings::isDisabled()
{
    return !is_enabled;
}

QColor ChannelSettings::niceRandomColor()
{
    return QColor ( qrand()%100 + 155, //r
                    qrand()%100 + 155, //g
                    qrand()%100 + 155  //b
                    );
}
