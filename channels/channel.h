#ifndef CHANNEL_H
#define CHANNEL_H

#include <QObject>

#include "ports/port.h"
#include "channels/channelsettings.h"

class Channel : public QObject
{
    Q_OBJECT

    PortPointer port;
    ChannelSettings settings;

public slots:
    void byteReceived( char byte )
    {
        emit gotByte(byte);
    }

signals:
    void gotByte( char byte );

public:
    Channel( PortPointer &port, ChannelSettings& settings )
        : port(port),
          settings(settings)
    { }

    ~Channel()
    { }

    ChannelSettings& getSettings()
    {
        return settings;
    }

    void setSettings(const ChannelSettings &value)
    {
        settings = value;
    }

    PortPointer getPort()
    {
        return port;
    }

    void start()
    {
        try {
            port->enable();
        } catch( QString &error_string ) {
            throw;
        }
    }

    void stop()
    {
        port->disable();
    }
};

#endif // CHANNEL_H
