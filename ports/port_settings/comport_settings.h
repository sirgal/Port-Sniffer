#ifndef COMPORT_SETTINGS_H
#define COMPORT_SETTINGS_H

#include "ports/port_settings/portsettings.h"

class ComPort;

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

    ComPortSettings& operator=( const ComPortSettings &port )
    {
        setName( port.getName() );
        baud_rate = port.baud_rate;
        return *this;
    }

    ComPortSettings& operator=( ComPortSettings &port )
    {
        setName( port.getName() );
        baud_rate = port.baud_rate;
        return *this;
    }

    int getBaudRate() const
    {
        return baud_rate;
    }

    void setBaudRate(int value)
    {
        baud_rate = value;
    }

    PortTypes getPortType() const
    {
        return PortTypes::ComPort;
    }

    PortPointer buildAccordingPort()
    {
        return std::dynamic_pointer_cast<Port>(std::make_shared<ComPort>( *this ));
    }
};

#endif // COMPORT_SETTINGS_H

