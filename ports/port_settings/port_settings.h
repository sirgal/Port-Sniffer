#ifndef PORTSETTINGS_H
#define PORTSETTINGS_H

#include <QString>

#include "ports/port.h"

class PortSettings
{
    QString name;

public:
    PortSettings( QString name ) :
        name(name)
    { }

    virtual ~PortSettings()
    { }

    QString getName();

    void setName( const QString &value );

    virtual PortPointer buildAccordingPort() = 0;
};

#endif // PORTSETTINGS_H






