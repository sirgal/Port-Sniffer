#include "ports/comport.h"

void ComPort::byteReady()
{
    char byte;

    while( com_port.read( &byte, 1 ) == 1 )
        emit gotByte( byte );
}

void ComPort::putByte(char byte)
{
    com_port.write(&byte, 1);
}

ComPort::ComPort(ComPort::settings_type settings) : settings(settings)
{
    com_port.setPortName( settings.getName() );
    com_port.setBaudRate( settings.getBaudRate() );
    connect( &com_port, SIGNAL(readyRead()), this, SLOT(byteReady()) );
}

ComPort::~ComPort()
{
    disconnect( &com_port, SIGNAL(readyRead()), this, SLOT(byteReady()) );
}

void ComPort::enable()
{
    com_port.clear();

    if( !com_port.open( QSerialPort::ReadWrite ) )
        throw com_port.errorString();
}

void ComPort::disable()
{
    com_port.close();
}

ComPort::settings_type &ComPort::getSettings()
{
    return settings;
}

void ComPort::setSettings( PortSettings &in_settings )
{
    try {
        settings = dynamic_cast<settings_type&>( in_settings );
    } catch(...) {
        throw;
    }

    com_port.setBaudRate( settings.getBaudRate() );
    com_port.setPortName( settings.getName() );
}
