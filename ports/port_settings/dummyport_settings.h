#ifndef DUMMYPORT_SETTINGS_H
#define DUMMYPORT_SETTINGS_H

#include "ports/port_settings/port_settings.h"

class DummyPortSettings : public PortSettings
{
public:
    DummyPortSettings() :
        PortSettings("DUMMY")
    { }

    PortPointer buildAccordingPort();

    static QString getTypeName()
    {
        return QString("Select...");
    }
};

#endif // DUMMYPORT_SETTINGS_H
