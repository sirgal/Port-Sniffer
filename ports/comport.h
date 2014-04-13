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
    virtual void byteReady()
    {
        char byte;

        while( com_port.read( &byte, 1 ) == 1 )
            emit gotByte( byte );
    }

    virtual void putByte(char byte)
    {
        emit gotByte( byte );
    }

public:
    ComPort( settings_type settings ) : settings(settings)
    {
        com_port.setPortName( settings.getName() );
        com_port.setBaudRate( settings.getBaudRate() );
        connect( &com_port, SIGNAL(readyRead()), this, SLOT(byteReady()) );
    }

    ~ComPort()
    {
        disconnect( &com_port, SIGNAL(readyRead()), this, SLOT(byteReady()) );
    }

    virtual void enable()
    {
        com_port.clear();

        if( !com_port.open( QSerialPort::ReadOnly ) )
            throw com_port.errorString();
    }

    virtual void disable()
    {
        com_port.close();
    }

    virtual const settings_type& getSettings()
    {
        return settings;
    }

    virtual void setSettings( const PortSettings& in_settings )
    {
        try {
            settings = dynamic_cast<const settings_type&>( in_settings );
        } catch(...) {
            throw;
        }

        com_port.setBaudRate( settings.getBaudRate() );
        com_port.setPortName( settings.getName() );
    }

    virtual PortTypes getPortType() const
    {
        return PortTypes::ComPort;
    }
};

#endif // COMPORT_H
