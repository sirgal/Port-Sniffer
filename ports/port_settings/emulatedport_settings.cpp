#include "ports/port_settings/emulatedport_settings.h"
#include "ports/emulatedport.h"

EmulatedPortSettings &EmulatedPortSettings::operator=(EmulatedPortSettings &port)
{
    setName( port.getName() );
    return *this;
}

PortPointer EmulatedPortSettings::buildAccordingPort()
{
    return std::dynamic_pointer_cast<Port>(std::make_shared<EmulatedPort>( *this, 1 ));
}

QString EmulatedPortSettings::getTypeName()
{
    return QString("Emulated port");
}
