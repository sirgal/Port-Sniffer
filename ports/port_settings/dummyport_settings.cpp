#include "dummyport_settings.h"
// dependancy-solving hack
#include "ports/dummyport.h"

PortPointer DummyPortSettings::buildAccordingPort()
{
    return std::dynamic_pointer_cast<Port>(std::make_shared<DummyPort>( *this ));
}
