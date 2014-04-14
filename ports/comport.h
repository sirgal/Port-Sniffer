#ifndef COMPORT_H
#define COMPORT_H

#include <memory>

#include <QString>
#include <QSerialPort>

#include "ports/port.h"
#include "ports/port_settings/comport_settings.h"

class ComPort : public Port
{
    Q_OBJECT

public:
    using settings_type = ComPortSettings;

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

    virtual PortTypes getPortType();
};

#endif // COMPORT_H
