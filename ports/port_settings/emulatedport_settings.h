#ifndef EMULATEDPORT_SETTINGS_H
#define EMULATEDPORT_SETTINGS_H

#include "ports/port_settings/port_settings.h"

class EmulatedPort;

class EmulatedPortSettings : public PortSettings
{
public:
    EmulatedPortSettings& operator=( EmulatedPortSettings &port )
    {
        setName( port.getName() );
        return *this;
    }

    PortTypes getPortType()
    {
        return PortTypes::EmulatedPort;
    }

    PortPointer buildAccordingPort()
    {
        return std::dynamic_pointer_cast<Port>(std::make_shared<EmulatedPort>( *this ));
    }
};

#endif // EMULATEDPORT_SETTINGS_H
