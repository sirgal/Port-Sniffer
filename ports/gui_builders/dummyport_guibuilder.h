#ifndef DUMMYPORT_GUIBUILDER_H
#define DUMMYPORT_GUIBUILDER_H

#include <QLayout>

#include "ports/gui_builders/port_guibuilder.h"
#include "ports/port_settings/dummyport_settings.h"

class DummyPortGuiBuilder: public PortGuiBuilder
{
    Q_OBJECT

public:
    DummyPortGuiBuilder()
    { }
    virtual ~DummyPortGuiBuilder()
    { }

    virtual void setSettings( PortSettings &settings )
    { Q_UNUSED(settings); }
    virtual void setSettings( PortSettingsPointer &settings )
    { Q_UNUSED(settings); }
    virtual PortSettingsPointer getSettings()
    {
        return std::dynamic_pointer_cast<PortSettings>(
                    std::make_shared<DummyPortSettings>()
                    );
    }

    virtual void buildForm( QFormLayout *layout )
    { Q_UNUSED(layout); }
    virtual QString getTypeName()
    { return DummyPortSettings().getTypeName(); }
};

#endif // DUMMYPORT_GUIBUILDER_H
