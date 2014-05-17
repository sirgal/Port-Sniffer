#ifndef COMPORT_SETTINGS_H
#define COMPORT_SETTINGS_H

#include "ports/port_settings/port_settings.h"

class ComPortSettings : public PortSettings
{
    int baud_rate;

public:
    ComPortSettings( QString name, int baud_rate ) :
        PortSettings(name),
        baud_rate(baud_rate)
    { }

    ComPortSettings &operator=( ComPortSettings &port );

    int getBaudRate() const;
    void setBaudRate(int value);

    PortPointer buildAccordingPort();

    virtual QString getTypeName()
    {
        return QString("COM port");
    }
};

#endif // COMPORT_SETTINGS_H
