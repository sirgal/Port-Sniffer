#ifndef CHANNELSETTINGS_H
#define CHANNELSETTINGS_H

#include <QString>
#include <QColor>
#include <QtSerialPort/QSerialPort>

#include "ports/port.h"

class ChannelSettings
{
    QColor port_color;
    int channel_number = 0;
    bool is_enabled = true;

public:
    ChannelSettings()
        : port_color(niceRandomColor())
    { }

    ChannelSettings( uint number )
        : port_color(niceRandomColor()),
          channel_number(number)
    { }

    void setEnabled( bool is_enabled );

    void setColor( QColor &new_port_color );
    QColor getColor();

    int getNumber();

    bool isEnabled();

    static QColor niceRandomColor();
};

#endif // CHANNELSETTINGS_H




