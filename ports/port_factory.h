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

protected:
    PortPointer appendPort( PortPointer port );

public:
    PortFactory() { }

    PortPointer buildPort( PortSettings& settings );
    void deletePort( PortPointer port );

    QList<PortPointer> getAllPorts();

    void startAllPorts();
    void stopAllPorts();
};

#endif // PORTFACTORY_H
