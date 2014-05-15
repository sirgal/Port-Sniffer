#include "channels/channel_settings.h"

void ChannelSettings::setEnabled(bool is_enabled)
{
    this->is_enabled = is_enabled;
}

void ChannelSettings::setColor(QColor &new_port_color)
{
    port_color = new_port_color;
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

QColor ChannelSettings::niceRandomColor()
{
    return QColor ( qrand()%100 + 155, //r
                    qrand()%100 + 155, //g
                    qrand()%100 + 155  //b
                    );
}
