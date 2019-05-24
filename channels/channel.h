#ifndef CHANNEL_H
#define CHANNEL_H

#include <QObject>

#include "ports/port.h"
#include "channels/channel_settings.h"

class Channel : public QObject
{
    Q_OBJECT

    PortSettingsPointer port_settings = nullptr;
    PortPointer active_port = nullptr;
    ChannelSettings settings;

public slots:
    void putByte( char byte );

private slots:
    void byteReceived( char byte );

signals:
    void gotByte( char byte );

public:
    Channel( ChannelSettings& settings )
        : settings(settings)
    { }

    ~Channel()
    { }

    ChannelSettings& getSettings();
    void setSettings(const ChannelSettings &value);

    PortSettingsPointer getPortSettings();
    void setPortSettings( PortSettingsPointer settings );

    void start();
    void stop();
};

#endif // CHANNEL_H
