#ifndef EMULATEDPORT_SETTINGS_H
#define EMULATEDPORT_SETTINGS_H

#include "ports/port_settings/portsettings.h"

class EmulatedPort;

class EmulatedPortSettings : public PortSettings
{
public:
    EmulatedPortSettings& operator=( const EmulatedPortSettings &port )
    {
        setName( port.getName() );
        return *this;
    }

    EmulatedPortSettings& operator=( EmulatedPortSettings &port )
    {
        setName( port.getName() );
        return *this;
    }

    PortTypes getPortType() const
    {
        return PortTypes::EmulatedPort;
    }

    PortPointer buildAccordingPort()
    {
        return std::dynamic_pointer_cast<Port>(std::make_shared<EmulatedPort>( *this ));
    }
};

#endif // EMULATEDPORT_SETTINGS_H
