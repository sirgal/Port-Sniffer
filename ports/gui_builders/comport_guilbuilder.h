#ifndef COMPORTGUILBUILDER_H
#define COMPORTGUILBUILDER_H

#include <QStringList>
#include <QComboBox>
#include <QHBoxLayout>
#include <QSerialPortInfo>

#include "ports/gui_builders/port_guibuilder.h"
#include "ports/port_settings/comport_settings.h"

class ComPortGuiBuilder : public PortGuiBuilder
{
    Q_OBJECT

    const QStringList available_speeds =
    {
        "2400",  "4800",   "9600",   "14400",
        "19200", "28800",  "38400",  "56000",
        "57600", "115200", "128000", "256000"
    };

    QHBoxLayout name_and_button;
    QComboBox port_name;
    QPushButton rescan_button;
    QComboBox port_speeds;

    QSerialPortInfo port_info;

    int getSpeed();
    QString getName();
    QStringList getAvailablePorts();

public slots:
    void rescanPorts();

public:
    ComPortGuiBuilder();
    virtual ~ComPortGuiBuilder() { }

    virtual void setSettings( PortSettingsPointer &settings );
    virtual PortSettingsPointer getSettings();

    virtual void buildForm( QFormLayout &layout );
    virtual QString getTypeName();
};

#endif // COMPORTGUILBUILDER_H
