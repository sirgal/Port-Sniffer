#ifndef PORTGUIBUILDER_H
#define PORTGUIBUILDER_H

#include <QFormLayout>

#include "ports/port_settings/port_settings.h"

class PortGuiBuilder
{
protected:
    QFormLayout *layout;

    void cleanLayout()
    {
        while( layout->layout()->count() != 0 )
            layout->takeAt(0);
    }

public:
    PortGuiBuilder( QFormLayout *layout ) :
        layout(layout)
    { }

    virtual void setSettings( PortSettingsPointer settings ) = 0;
    virtual PortSettingsPointer getSettings() = 0;
    virtual void buildForm() = 0;
};

#endif // PORTGUIBUILDER_H
