#include "ports/port_factory.h"

PortPointer PortFactory::appendPort(PortPointer port)
{
    if( !port )
        throw std::bad_cast(/*insanity!*/);

    ports.append( port );

    return port;
}

PortPointer PortFactory::buildPort(PortSettings &settings)
{
    return appendPort( settings.buildAccordingPort() );
}

void PortFactory::deletePort(PortPointer port)
{
    port->disable();
    ports.removeAll( port );
}

QList<PortPointer> PortFactory::getAllPorts()
{
    return ports;
}

void PortFactory::startAllPorts()
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

void PortFactory::stopAllPorts()
{
    foreach( PortPointer port, ports )
        port->disable();
}
