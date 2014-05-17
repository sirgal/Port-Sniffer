#ifndef DUMMYPORT_SETTINGS_H
#define DUMMYPORT_SETTINGS_H

#include "ports/port_settings/port_settings.h"

class DummyPortSettings : public PortSettings
{
public:
    DummyPortSettings() :
        PortSettings("DUMMY")
    { }

    virtual PortPointer buildAccordingPort(int chan_num);

    virtual QString getTypeName()
    {
        return QString("Select...");
    }
};

#endif // DUMMYPORT_SETTINGS_H
