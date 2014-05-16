#ifndef EMULATEDPORTGUIBUILDER_H
#define EMULATEDPORTGUIBUILDER_H

#include <QLayout>

#include "ports/gui_builders/port_guibuilder.h"
#include "ports/port_settings/emulatedport_settings.h"

class EmulatedPortGuiBuilder: public PortGuiBuilder
{
    Q_OBJECT

public:
    EmulatedPortGuiBuilder()
    { }
    virtual ~EmulatedPortGuiBuilder()
    { }

    virtual void setSettings( PortSettings &settings )
    { Q_UNUSED(settings); }
    virtual void setSettings( PortSettingsPointer &settings )
    { Q_UNUSED(settings); }
    virtual PortSettingsPointer getSettings()
    {
        return std::dynamic_pointer_cast<PortSettings>(
                    std::make_shared<EmulatedPortSettings>()
                    );
    }

    virtual void buildForm( QFormLayout *layout )
    { Q_UNUSED(layout);

    }
    virtual QString getTypeName()
    { return EmulatedPortSettings::getTypeName(); }
};
#endif // EMULATEDPORTGUIBUILDER_H
