#ifndef COMPORTGUILBUILDER_H
#define COMPORTGUILBUILDER_H

#include <QStringList>
#include <QComboBox>
#include <QSerialPortInfo>

#include "ports/gui_builders/portguibuilder.h"
#include "ports/port_settings/comport_settings.h"

class ComPortGuiBuilder : public PortGuiBuilder
{
    Q_OBJECT

    QStringList available_speeds = { "2400",  "4800",   "9600",   "14400",
                                     "19200", "28800",  "38400",  "56000",
                                     "57600", "115200", "128000", "256000" };

    QComboBox port_name;
    QComboBox port_speeds;

    QSerialPortInfo port_info;

    int getSpeed()
    {
        return port_speeds.currentText().toInt();
    }

    QString getName()
    {
        return port_name.currentText();
    }

    QStringList getAvailablePorts()
    {
        QStringList string_list;
        auto port_list = port_info.availablePorts();

        foreach( QSerialPortInfo port, port_list )
            string_list.append( port.portName() );

        return string_list;
    }

public slots:

signals:

public:
    ComPortGuiBuilder( QFormLayout *layout ) :
        PortGuiBuilder(layout)
    {
        port_speeds.addItems( available_speeds );
    }

    virtual PortSettingsPointer getSettings()
    {
        return std::dynamic_pointer_cast<PortSettings>(
                    std::make_shared<ComPortSettings>( getName(), getSpeed() )
                    );
    }

    virtual void buildForm()
    {
        cleanLayout();

        layout->addRow( "Port name: ", &port_name );
        layout->addRow( "Port name: ", &port_speeds );
    }
};

#endif // COMPORTGUILBUILDER_H
