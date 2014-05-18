#ifndef EMULATEDPORT_H
#define EMULATEDPORT_H

#include <QMap>
#include <QDebug>
#include <QThread>

#include "ports/port.h"
#include "port_settings/emulatedport_settings.h"
#include "gui_builders/emulatedport_guibuilder.h"

class EmulatedPort : public Port
{
    Q_OBJECT

public:
    using settings_type = EmulatedPortSettings;
    using gui_builder_type = EmulatedPortGuiBuilder;

    class EmulatedPortFeeder : public QThread
    {
        QMap<EmulatedPort*, int> port_to_num;
        QMap<int, EmulatedPort*> num_to_port;
        std::shared_ptr<QFile> file = nullptr;
        int ready_count = 0;
        bool running = false;
    public:
        EmulatedPortFeeder()
            : QThread()
        { }

        void registerMe( int chan_num, EmulatedPort* who )
        {
            port_to_num[who] = chan_num;
            qDebug() << "Regged " << chan_num;
        }

        void deRegisterMe( EmulatedPort* who )
        {
            qDebug() << "De-Regged " << port_to_num.value(who);
            port_to_num.remove( who );
        }

        void imReady( EmulatedPort* who )
        {
            if( !port_to_num.contains( who ) )
                return;

            // all of them are static and the same
            file = who->file;

            ready_count++;
            // every registered port is ready?
            // then start feeding
            if( ready_count == port_to_num.count() ) {
                // build reverse map
                num_to_port.clear();
                foreach( EmulatedPort* port, port_to_num.keys() ) {
                    num_to_port[port_to_num.value(port)] = port;
                }

                start();
            }
        }

        void imaStopping( EmulatedPort* who )
        {
            if( !port_to_num.contains( who ) )
                return;

            if( ready_count > 0 )
                ready_count--;

            running = false;
        }

        void stopAll()
        {
            ready_count = 0;
            running = false;
        }

        void run()
        {
            // feeding procedure
            running = true;
            // take any file path - they all should be same
            qDebug() << "BEING RUN";

            if( !file )
                running = false;

            while( running ) {
                const int BUF_SIZE = 4096;
                char content[BUF_SIZE];
                long long int read = 0;

                read = file->read( content, BUF_SIZE );

                if( read < 1 )
                    running = false;

                for( int i = 0; ( i < read / 2 ) && running; i++ ) {
                    char channel = content[i*2];
                    char data    = content[i*2 + 1];

                    EmulatedPort* port = num_to_port.value( channel, nullptr );

                    if( !port )
                        continue;

                    port->putByte( data );
                }
            }
            qDebug() << "STOPPED";
        }
    };

    friend class EmulatedPortFeeder;

private:
    settings_type settings;
    static EmulatedPortFeeder feeder;
    static std::shared_ptr<QFile> file;
    int my_channel = -1;

public slots:
    virtual void byteReady()
    {

    }

    virtual void putByte(char byte)
    {
        qDebug() << "Got byte! " << my_channel << (int) byte;
        emit gotByte(byte);
    }
public:
    EmulatedPort( settings_type settings, int chan_num )
        : settings(settings), my_channel(chan_num)
    {
        feeder.registerMe( chan_num, this );
    }

    ~EmulatedPort()
    {
        feeder.deRegisterMe( this );
    }

    virtual void enable()
    {
        if( !file ) {
            file = std::make_shared<QFile>( settings.getFilePath() );

            if( !file->open( QIODevice::ReadOnly ) ) {
                feeder.stopAll();
                throw QString("Emulated port failed to open file");
            }
        }

        feeder.imReady(this);
    }

    virtual void disable()
    {
        feeder.imaStopping( this );

        // close file on our side, feeder should be stopped already
        if( file )
            file = nullptr;
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
