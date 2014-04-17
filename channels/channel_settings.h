#ifndef CHANNELSETTINGS_H
#define CHANNELSETTINGS_H

#include <QString>
#include <QColor>
#include <QtSerialPort/QSerialPort>

#include "ports/port.h"

class ChannelSettings
{
    QColor port_color;
    QString port_type_name;
    int channel_number;
    bool is_enabled;

public:
    ChannelSettings()
        : port_color(niceRandomColor()),
          channel_number(0),
          is_enabled(true)
    { }

    ChannelSettings( uint number )
        : port_color(niceRandomColor()),
          channel_number(number),
          is_enabled(true)
    { }

    void enable();
    void disable();

    void setColor( QColor &new_port_color );
    QColor getColor();

    int getNumber();

    bool isEnabled();
    bool isDisabled();

    static QColor niceRandomColor();
    QString getPortTypeName();
    void setPortTypeName( QString &value );
};

#endif // CHANNELSETTINGS_H




