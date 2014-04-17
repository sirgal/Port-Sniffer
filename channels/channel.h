#ifndef CHANNEL_H
#define CHANNEL_H

#include <QObject>

#include "ports/port.h"
#include "channels/channel_settings.h"

class Channel : public QObject
{
    Q_OBJECT

    PortPointer port;
    ChannelSettings settings;

public slots:
    void byteReceived( char byte );

signals:
    void gotByte( char byte );

public:
    Channel( PortPointer &port, ChannelSettings& settings )
        : port(port),
          settings(settings)
    { }

    ~Channel()
    { }

    ChannelSettings& getSettings();
    void setSettings(const ChannelSettings &value);

    PortPointer getPort();
    void setPortSettings( PortSettingsPointer settings );

    void start();
    void stop();
};

#endif // CHANNEL_H
