#ifndef DUMMYPORT_SETTINGS_H
#define DUMMYPORT_SETTINGS_H

#include "ports/port_settings/port_settings.h"

class DummyPortSettings : public PortSettings
{
public:
    DummyPortSettings()
    { }

    PortPointer buildAccordingPort();

    static QString getTypeName()
    {
        return QString("None");
    }
};

#endif // DUMMYPORT_SETTINGS_H
