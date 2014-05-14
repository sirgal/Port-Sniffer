#include "settingguiwireup.h"

SettingGuiWireup::SettingGuiWireup(
        QComboBox *port_type_combobox,
        QFormLayout *port_settings_frame,
        QListWidget *channel_list )
    : port_type_combobox(port_type_combobox),
      port_settings_frame(port_settings_frame),
      channel_list(channel_list)
{
    connect( port_type_combobox, SIGNAL(currentIndexChanged(QString)), this, SLOT(onPortTypeChange(QString)) );
    connect( channel_list, SIGNAL(currentTextChanged(QString)), this, SLOT(onChannelChange(QString)) );
}

void SettingGuiWireup::registerPortType(PortGuiBuilderPointer builder)
{
    QString type_name = builder->getTypeName();
    type_to_builder[type_name] = builder;
    port_type_combobox->addItem(type_name);
}

void SettingGuiWireup::onPortTypeChange(QString new_type)
{
    PortGuiBuilderPointer builder = type_to_builder.value(new_type);
    builder->buildForm( port_settings_frame );
}

void SettingGuiWireup::onChannelChange( QString new_channel )
{
    if( current ) {
        emit storeSettings(current->getSettings(), last_channel);
    }

    last_channel = new_channel.toInt();

    PortSettingsPointer settings =
            channels->getChannelSettings( last_channel );

    if( !settings )
        return;

    current = type_to_builder.value( settings->getTypeName() );

    if( !current )
        return;

    current->buildForm( port_settings_frame );
}

void SettingGuiWireup::setSettings(PortSettingsPointer settings)
{
    current->setSettings( settings );
}
