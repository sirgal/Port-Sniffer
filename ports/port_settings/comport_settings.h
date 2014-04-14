#ifndef COMPORT_SETTINGS_H
#define COMPORT_SETTINGS_H

#include "ports/port_settings/portsettings.h"

class ComPortSettings : public PortSettings
{
    int baud_rate;
public:
    ComPortSettings( QString name, int baud_rate ) :
        PortSettings(name),
        baud_rate(baud_rate)
    { }

    ComPortSettings( ComPortSettings &port ) :
        PortSettings(port.getName()),
        baud_rate(port.baud_rate)
    { }

    ComPortSettings( const ComPortSettings &port ) :
        PortSettings(port.getName()),
        baud_rate(port.baud_rate)
    { }

    ComPortSettings &operator=( const ComPortSettings &port );

    ComPortSettings &operator=( ComPortSettings &port );

    int getBaudRate() const;

    void setBaudRate(int value);

    PortTypes getPortType() const;

    PortPointer buildAccordingPort();
};

#endif // COMPORT_SETTINGS_H
