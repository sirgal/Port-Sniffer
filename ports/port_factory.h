#ifndef PORTFACTORY_H
#define PORTFACTORY_H

#include <memory>

#include "ports/port.h"
#include "ports/comport.h"
#include "ports/emulatedport.h"

class PortFactory
{
public:
    PortFactory() { }

    PortPointer buildPort( PortSettings& settings );
};

#endif // PORTFACTORY_H
