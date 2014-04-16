#ifndef GUIBUILDER_FACTORY_H
#define GUIBUILDER_FACTORY_H

#include <QMap>
#include <QString>
#include <QStringList>
#include <QFormLayout>

#include "ports/gui_builders/port_guibuilder.h"
#include "ports/gui_builders/comport_guilbuilder.h"

class GuiBuilderFactory
{
    QMap<QString, PortGuiBuilderPointer> type_name_to_builder;
    QFormLayout *layout;
    PortGuiBuilderPointer current;
public:
    GuiBuilderFactory();

    void setLayout( QFormLayout *layout );

    void registerPortType(PortGuiBuilderPointer &builder );
    QStringList getAvailableTypes();

    void setType(QString type_name );
    QString getType();

    void setSettings( PortSettingsPointer &settings );
    PortSettingsPointer getSettings();
};

#endif // GUIBUILDER_FACTORY_H
