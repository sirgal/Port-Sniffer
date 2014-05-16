#ifndef EMULATEDPORT_SETTINGS_H
#define EMULATEDPORT_SETTINGS_H

#include "ports/port_settings/port_settings.h"

class EmulatedPortSettings : public PortSettings
{
public:
    EmulatedPortSettings &operator=( EmulatedPortSettings &port );

    PortPointer buildAccordingPort();

    virtual QString getTypeName();
};



#endif // EMULATEDPORT_SETTINGS_H
