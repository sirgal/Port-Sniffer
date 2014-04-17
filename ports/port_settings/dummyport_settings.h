#ifndef DUMMYPORT_SETTINGS_H
#define DUMMYPORT_SETTINGS_H

#include "ports/port_settings/port_settings.h"

class DummyPortSettings : public PortSettings
{
public:
    DummyPortSettings()
    { }

    PortPointer buildAccordingPort();
};

#endif // DUMMYPORT_SETTINGS_H
