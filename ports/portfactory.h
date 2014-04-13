#ifndef PORTFACTORY_H
#define PORTFACTORY_H

#include <QList>
#include <QObject>
#include <QString>

#include <memory>

#include "ports/port.h"
#include "ports/comport.h"
#include "ports/emulatedport.h"

class PortFactory : public QObject
{
    Q_OBJECT
    QList<PortPointer> ports;

    PortPointer appendPort( PortPointer port )
    {
        if( !port )
            throw std::bad_cast(/*insanity!*/);

        ports.append( port );

        return port;
    }

public:
    PortFactory() { }

    PortPointer buildPort( PortSettings& settings )
    {
        return appendPort( settings.buildAccordingPort() );
    }

    void deletePort( PortPointer port )
    {
        port->disable();
        ports.removeAll( port );
    }

    QList<PortPointer> getAllPorts()
    {
        return ports;
    }

    void startAllPorts()
    {
        foreach( PortPointer port, ports ) {
            try {
                port->enable();
            } catch( QString &error_string ) {
                QString error("An error occured on port: ");
                // I believe, port names will rarely be longer than 20 chars
                // but if they will - rightmost 20 chars would be enough, right?
                error += port->getSettings().getName().right(20);
                error += "\nError: " + error_string;
                throw error;
            }
        }
    }

    void stopAllPorts()
    {
        foreach( PortPointer port, ports )
            port->disable();
    }
};

#endif // PORTFACTORY_H
