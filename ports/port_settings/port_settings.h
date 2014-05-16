#ifndef PORTSETTINGS_H
#define PORTSETTINGS_H

#include <QString>

#include "ports/port.h"

class PortSettings
{
    QString name;

public:
    PortSettings() :
        name("INVALID")
    { }

    PortSettings( QString name ) :
        name(name)
    { }

    virtual ~PortSettings()
    { }

    static QString getTypeName()
    {
        return QString("LOL U FAILED!");
    }

    QString getName();

    void setName( const QString &value );

    virtual PortPointer buildAccordingPort() = 0;
};

#endif // PORTSETTINGS_H






