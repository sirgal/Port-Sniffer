#ifndef PORT_H
#define PORT_H

#include <memory>

#include <QString>
#include <QObject>

class Port;
class PortSettings;
class PortGuiBuilder;

using PortPointer = std::shared_ptr<Port>;

#include "ports/port_settings/port_settings.h"

using PortSettingsPointer = std::shared_ptr<PortSettings>;

#include "gui_builders/port_guibuilder.h"

class Port : public QObject
{
    Q_OBJECT
public:
    using settings_type = PortSettings;
    using gui_builder_type = PortGuiBuilder;
public slots:
    virtual void byteReady() = 0;
    virtual void putByte(char byte) = 0;
signals:
    void gotByte(char byte);
public:
    Port() { }
    virtual ~Port() { }

    virtual void enable() = 0;
    virtual void disable() = 0;
    virtual settings_type& getSettings() = 0;
    virtual void setSettings( PortSettings& in_settings ) = 0;
};

#endif // PORT_H
