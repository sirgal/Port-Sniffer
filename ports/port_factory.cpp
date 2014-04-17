#include "ports/port_factory.h"

PortPointer PortFactory::buildPort(PortSettings &settings)
{
    return settings.buildAccordingPort();
}
