#ifndef EMULATEDPORT_SETTINGS_H
#define EMULATEDPORT_SETTINGS_H

#include <QString>

#include "ports/port_settings/port_settings.h"

class EmulatedPortSettings : public PortSettings
{
    QString file_path;
public:
    EmulatedPortSettings &operator=( EmulatedPortSettings &port );

    virtual PortPointer buildAccordingPort(int chan_num);

    virtual QString getTypeName();

    void setFilePath(QString file_path);
    QString getFilePath();
};

#endif // EMULATEDPORT_SETTINGS_H
