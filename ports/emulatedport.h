#ifndef EMULATEDPORT_H
#define EMULATEDPORT_H

#include "ports/port.h"
#include "port_settings/emulatedport_settings.h"

class EmulatedPort : public Port
{
    Q_OBJECT
public:
    using settings_type = EmulatedPortSettings;
private:
    settings_type settings;
public slots:
    virtual void byteReady()
    {

    }

    virtual void putByte(char byte)
    {
        emit gotByte(byte);
    }
signals:
    void gotByte(char byte);
public:
    EmulatedPort( settings_type settings ) : settings(settings) { }
    ~EmulatedPort() { }

    virtual void enable()
    {

    }

    virtual void disable()
    {

    }

    virtual settings_type& getSettings()
    {
        return settings;
    }

    virtual void setSettings( PortSettings& in_settings )
    {
        try {
            settings = dynamic_cast<settings_type&>( in_settings );
        } catch(...) {
            throw;
        }
    }

    virtual PortTypes getPortType()
    {
        return PortTypes::EmulatedPort;
    }
};

#endif // EMULATEDPORT_H
