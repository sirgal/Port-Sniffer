#ifndef PORTGUIBUILDER_H
#define PORTGUIBUILDER_H

#include <QObject>
#include <QFormLayout>

#include "ports/port_settings/portsettings.h"

class PortGuiBuilder : public QObject
{
    Q_OBJECT
protected:
    QFormLayout *layout;

    void cleanLayout()
    {
        while( layout->layout()->count() != 0 )
            layout->takeAt(0);
    }
public slots:

signals:

public:
    PortGuiBuilder( QFormLayout *layout ) :
        layout(layout)
    { }

    virtual PortSettingsPointer getSettings() = 0;
    virtual void buildForm() = 0;
};

#endif // PORTGUIBUILDER_H
