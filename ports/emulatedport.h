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
    }

    virtual PortTypes getPortType() const
    {
        return PortTypes::EmulatedPort;
    }
};

#endif // EMULATEDPORT_H
