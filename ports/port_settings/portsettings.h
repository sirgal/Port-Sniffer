#ifndef PORTSETTINGS_H
#define PORTSETTINGS_H

#include <QString>

#include "ports/port.h"

class PortSettings
{
    QString name;

public:
    PortSettings( QString name ) : name(name) { }
    virtual ~PortSettings() { }

    QString getName() const
    {
        return name;
    }

    void setName(const QString &value)
    {
        name = value;
    }

    virtual PortTypes getPortType() const = 0;
    virtual PortPointer buildAccordingPort() = 0;
};

#endif // PORTSETTINGS_H






