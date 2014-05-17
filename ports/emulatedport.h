#ifndef EMULATEDPORT_H
#define EMULATEDPORT_H

#include <QMap>
#include <QDebug>

#include "ports/port.h"
#include "port_settings/emulatedport_settings.h"
#include "gui_builders/emulatedport_guibuilder.h"

class EmulatedPort : public Port
{
    Q_OBJECT

public:
    using settings_type = EmulatedPortSettings;
    using gui_builder_type = EmulatedPortGuiBuilder;

    class EmulatedPortFeeder
    {
        QMap<EmulatedPort*, int> port_to_num;
        int ready_count = 0;
    public:
        EmulatedPortFeeder()
        { }

        void registerMe( int chan_num, EmulatedPort* who )
        {
            port_to_num[who] = chan_num;
            qDebug() << "registeded";
        }

        void deRegisterMe( EmulatedPort* who )
        {
            qDebug() << "deregisteded";
        }

        void imReady( EmulatedPort* who )
        {
            ready_count++;
        }

        void imaStopping( EmulatedPort* who )
        {
            ready_count--;
        }

        void runLater();
    };

private:
    settings_type settings;
    static EmulatedPortFeeder feeder;

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
    EmulatedPort( settings_type settings, int number )
        : settings(settings)
    {
        feeder.registerMe( number, this );
    }

    ~EmulatedPort()
    {
        feeder.deRegisterMe( this );
    }

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

    virtual QString getTypeName()
    {
        return "Emulated port";
    }
};

#endif // EMULATEDPORT_H
