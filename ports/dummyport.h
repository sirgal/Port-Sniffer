#ifndef DUMMYPORT_H
#define DUMMYPORT_H

#include "ports/port.h"
#include "ports/port_settings/dummyport_settings.h"
#include "ports/gui_builders/dummyport_guibuilder.h"

class DummyPort : public Port
{
    Q_OBJECT

public:
    using settings_type = DummyPortSettings;
    using gui_builder_type = DummyPortGuiBuilder;

private:
    settings_type settings;

public slots:
    virtual void byteReady() { }
    virtual void putByte(char byte)
    { Q_UNUSED(byte); }

public:
    DummyPort( settings_type settings )
    { Q_UNUSED(settings); }

    virtual void enable() { }
    virtual void disable() { }

    virtual settings_type& getSettings()
    { return settings; }
    virtual void setSettings(PortSettings &in_settings )
    { Q_UNUSED(in_settings); }
};

#endif // DUMMYPORT_H
