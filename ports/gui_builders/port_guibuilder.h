#ifndef PORTGUIBUILDER_H
#define PORTGUIBUILDER_H

#include <memory>

#include <QFormLayout>

#include "ports/port_settings/port_settings.h"

class PortGuiBuilder;

using PortGuiBuilderPointer = std::shared_ptr<PortGuiBuilder>;

class PortGuiBuilder
{
protected:
    static void cleanLayout( QFormLayout &layout )
    {
        while( layout.layout()->count() != 0 )
            layout.takeAt(0);
    }

public:
    PortGuiBuilder()
    { }

    virtual void setSettings( PortSettingsPointer &settings ) = 0;
    virtual PortSettingsPointer getSettings() = 0;

    virtual void buildForm( QFormLayout &layout ) = 0;
    virtual QString getTypeName() = 0;
};

#endif // PORTGUIBUILDER_H
