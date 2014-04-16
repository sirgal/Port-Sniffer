#include "ports/gui_builders/comport_guilbuilder.h"

int ComPortGuiBuilder::getSpeed()
{
    return port_speeds.currentText().toInt();
}

QString ComPortGuiBuilder::getName()
{
    return port_name.currentText();
}

QStringList ComPortGuiBuilder::getAvailablePorts()
{
    QStringList string_list;
    auto port_list = port_info.availablePorts();

    foreach( QSerialPortInfo port, port_list )
        string_list.append( port.portName() );

    return string_list;
}

void ComPortGuiBuilder::setSettings( PortSettingsPointer &in_settings )
{
    std::shared_ptr<ComPortSettings> settings =
            std::dynamic_pointer_cast<ComPortSettings>(in_settings);

    if( !settings )
        return;

    QString name = settings->getName();
    QString baud_rate = QString::number(settings->getBaudRate());

    for( int i = 0; i < port_name.count(); i++ ){
        if( port_name.itemText(i) == name ){
            port_name.setCurrentIndex( i );
            break;
        }
    }

    int speed_idx = available_speeds.indexOf( baud_rate );

    if( speed_idx != -1 )
        port_speeds.setCurrentIndex( speed_idx );
}

PortSettingsPointer ComPortGuiBuilder::getSettings()
{
    return std::dynamic_pointer_cast<PortSettings>(
                std::make_shared<ComPortSettings>( getName(), getSpeed() )
                );
}

void ComPortGuiBuilder::buildForm( QFormLayout &layout )
{
    cleanLayout( layout );

    layout.addRow( "Port name: ", &port_name );
    layout.addRow( "Port name: ", &port_speeds );
}

QString ComPortGuiBuilder::getTypeName()
{
    return "RS-232 port";
}
