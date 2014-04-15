#ifndef COMPORT_H
#define COMPORT_H

#include <memory>

#include <QString>
#include <QSerialPort>

#include "ports/port.h"
#include "ports/port_settings/comport_settings.h"
#include "ports/gui_builders/comport_guilbuilder.h"

class ComPort : public Port
{
    Q_OBJECT

public:
    using settings_type = ComPortSettings;
    using gui_builder_type = ComPortGuiBuilder;

private:
    QSerialPort com_port;
    settings_type settings;

public slots:
    virtual void byteReady();
    virtual void putByte(char byte);

public:
    ComPort( settings_type settings );

    ~ComPort();

    virtual void enable();
    virtual void disable();

    virtual settings_type& getSettings();
    virtual void setSettings(PortSettings &in_settings );
};

#endif // COMPORT_H
