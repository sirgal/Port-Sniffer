#include "ports/port_settings/comport_settings.h"
// dependancy-solving hack
#include "ports/comport.h"

ComPortSettings &ComPortSettings::operator=(ComPortSettings &port)
{
    setName( port.getName() );
    baud_rate = port.baud_rate;
    return *this;
}

int ComPortSettings::getBaudRate() const
{
    return baud_rate;
}

void ComPortSettings::setBaudRate(int value)
{
    baud_rate = value;
}

PortPointer ComPortSettings::buildAccordingPort()
{
    return std::dynamic_pointer_cast<Port>(std::make_shared<ComPort>( *this ));
}
