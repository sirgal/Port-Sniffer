#include "guibuilder_factory.h"

GuiBuilderFactory::GuiBuilderFactory()
{ }

void GuiBuilderFactory::setLayout(QFormLayout *layout)
{
    this->layout = layout;
}

void GuiBuilderFactory::registerPortType( PortGuiBuilderPointer &builder )
{
    type_name_to_builder[builder->getTypeName()] = builder;
}

QStringList GuiBuilderFactory::getAvailableTypes()
{
    return type_name_to_builder.keys();
}

void GuiBuilderFactory::setType( QString type_name )
{
    PortGuiBuilderPointer builder = type_name_to_builder.value( type_name );
    current = builder;

    if( !builder )
        return;

    builder->buildForm( *layout );
}

void GuiBuilderFactory::setSettings(PortSettingsPointer &settings)
{
    if( current )
        current->setSettings( settings );
}

PortSettingsPointer GuiBuilderFactory::getSettings()
{
    if( current )
        return current->getSettings();
    else
        return nullptr;
}
