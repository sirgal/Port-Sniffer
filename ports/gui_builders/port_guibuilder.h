#ifndef PORTGUIBUILDER_H
#define PORTGUIBUILDER_H

#include <memory>

#include <QPushButton>
#include <QFormLayout>
#include <QObject>

#include "ports/port_settings/port_settings.h"

class PortGuiBuilder;

using PortGuiBuilderPointer = std::shared_ptr<PortGuiBuilder>;

class PortGuiBuilder : public QObject
{
    Q_OBJECT

protected:
    void cleanLayout( QFormLayout &layout );

public:
    PortGuiBuilder()
    { }
    virtual ~PortGuiBuilder()
    { }

    virtual void setSettings( PortSettingsPointer &settings ) = 0;
    virtual PortSettingsPointer getSettings() = 0;

    virtual void buildForm( QFormLayout &layout ) = 0;
    virtual QString getTypeName() = 0;
};

#endif // PORTGUIBUILDER_H
