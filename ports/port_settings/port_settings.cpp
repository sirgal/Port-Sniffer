#include "ports/port_settings/port_settings.h"

QString PortSettings::getName()
{
    return name;
}

void PortSettings::setName(const QString &value)
{
    name = value;
}
