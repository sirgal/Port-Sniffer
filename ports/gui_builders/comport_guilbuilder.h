#ifndef COMPORTGUILBUILDER_H
#define COMPORTGUILBUILDER_H

#include <QStringList>
#include <QComboBox>
#include <QSerialPortInfo>

#include "ports/gui_builders/port_guibuilder.h"
#include "ports/port_settings/comport_settings.h"

class ComPortGuiBuilder : public PortGuiBuilder
{
    const QStringList available_speeds =
    {
        "2400",  "4800",   "9600",   "14400",
        "19200", "28800",  "38400",  "56000",
        "57600", "115200", "128000", "256000"
    };

    QComboBox port_name;
    QComboBox port_speeds;

    QSerialPortInfo port_info;

    int getSpeed();
    QString getName();
    QStringList getAvailablePorts();

public:
    ComPortGuiBuilder( QFormLayout *layout ) :
        PortGuiBuilder(layout)
    {
        port_speeds.addItems( available_speeds );
    }

    virtual void setSettings( PortSettingsPointer in_settings );
    virtual PortSettingsPointer getSettings();

    virtual void buildForm();
};

#endif // COMPORTGUILBUILDER_H
