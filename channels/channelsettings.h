#ifndef CHANNELSETTINGS_H
#define CHANNELSETTINGS_H

#include <QString>
#include <QColor>
#include <QtSerialPort/QSerialPort>

#include "ports/port.h"

class ChannelSettings
{
    QColor port_color;
    int channel_number;
    bool is_enabled;

public:
    ChannelSettings()
        : channel_number(0),
          is_enabled(true)
    {
        niceRandomColor( port_color );
    }

    ChannelSettings( uint number )
        : channel_number(number),
          is_enabled(true)
    {
        niceRandomColor( port_color );
    }

    void setColor( const QColor &new_port_color )
    {
        port_color = new_port_color;
    }

    void enable()
    {
        is_enabled = true;
    }
    void disable()
    {
        is_enabled = false;
    }

    QColor getColor()
    {
        return port_color;
    }

    int getNumber()
    {
        return channel_number;
    }

    bool isEnabled()
    {
        return is_enabled;
    }

    bool isDisabled()
    {
        return !is_enabled;
    }

    static void niceRandomColor( QColor &color )
    {
        color = QColor( qrand()%100 + 155, //r
                        qrand()%100 + 155, //g
                        qrand()%100 + 155  //b
                        );
    }
};

#endif // CHANNELSETTINGS_H




