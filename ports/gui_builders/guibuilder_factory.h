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
    QMap<QString, int> type_name_to_idx;
    QMap<int, PortGuiBuilderPointer> idx_to_builder;

    QFormLayout *layout;
    PortGuiBuilderPointer current_builder;

public:
    GuiBuilderFactory();

    void setLayout( QFormLayout *layout );

    void registerPortType( PortGuiBuilderPointer &builder );
    QStringList getAvailableTypes();
    int getTypeNameIndex( QString &type_name );

    void setType( QString type_name );
    QString getType();

    void setSettings( PortSettings &settings );
    void setSettings( PortSettingsPointer &settings );
    PortSettingsPointer getSettings();
};

#endif // GUIBUILDER_FACTORY_H
