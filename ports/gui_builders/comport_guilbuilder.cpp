#include "ports/gui_builders/comport_guilbuilder.h"

ComPortGuiBuilder::ComPortGuiBuilder()
{
    port_speeds.addItems( available_speeds );

    rescan_button.setText( "Rescan ports" );

    connect( &rescan_button, SIGNAL(clicked()), this, SLOT(rescanPorts()) );

    name_and_button.addWidget( &port_name );
    name_and_button.addWidget( &rescan_button );
}

void ComPortGuiBuilder::setSettings( PortSettings &settings )
{
    try {
        ComPortSettings &com_settings = dynamic_cast<ComPortSettings&>( settings );

        QString name = com_settings.getName();
        QString baud_rate = QString::number( com_settings.getBaudRate() );

        for( int i = 0; i < port_name.count(); i++ ){
            if( port_name.itemText(i) == name ){
                port_name.setCurrentIndex( i );
                break;
            }
        }

        int speed_idx = available_speeds.indexOf( baud_rate );

        if( speed_idx != -1 )
            port_speeds.setCurrentIndex( speed_idx );
    } catch(...) {
        return;
    }
}

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

void ComPortGuiBuilder::rescanPorts()
{
    port_name.clear();
    port_name.addItems( getAvailablePorts() );
}

void ComPortGuiBuilder::setSettings( PortSettingsPointer &in_settings )
{
    std::shared_ptr<ComPortSettings> settings =
            std::dynamic_pointer_cast<ComPortSettings>(in_settings);

    if( !settings )
        return;

    setSettings( *settings );
}

PortSettingsPointer ComPortGuiBuilder::getSettings()
{
    return std::dynamic_pointer_cast<PortSettings>(
                std::make_shared<ComPortSettings>( getName(), getSpeed() )
                );
}

void ComPortGuiBuilder::buildForm(QFormLayout *layout)
{
    layout->addRow( "Port name: ", &name_and_button );
    layout->addRow( "Port speed: ", &port_speeds );
}

QString ComPortGuiBuilder::getTypeName()
{
    return ComPortSettings::getTypeName();
}
