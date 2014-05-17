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

    virtual QString getTypeName()
    {
        return QString("LOL U FAIL!");
    }

    QString getName();

    void setName( const QString &value );

    virtual PortPointer buildAccordingPort(int chan_num) = 0;
};

#endif // PORTSETTINGS_H






