#include "settingguiwireup.h"

SettingGuiWireup::SettingGuiWireup(QWidget *parent)
    : QStackedWidget(parent)
{ }

void SettingGuiWireup::registerFriends(QComboBox *port_type_combobox,
                                       QListWidget *channel_list,
                                       ChannelFactory *channels)
{
    this->port_type_combobox = port_type_combobox;
    this->channel_list = channel_list;
    this->channels = channels;

    connect( port_type_combobox, SIGNAL(currentIndexChanged(QString)), this, SLOT(onPortTypeChange(QString)) );
    connect( channel_list, SIGNAL(currentTextChanged(QString)), this, SLOT(onChannelChange(QString)) );

    valid = true;
}

void SettingGuiWireup::registerPortType( PortGuiBuilderPointer builder )
{
    if( !valid )
        return;

    QFrame *new_frame = new QFrame( this );
    QFormLayout *new_layout = new QFormLayout( new_frame );
    new_layout->setContentsMargins(0,0,0,0);

    QString type_name = builder->getTypeName();
    builder->buildForm( new_layout );

    type_to_builder[type_name] = builder;
    type_to_index[type_name] = addWidget( new_frame );

    port_type_combobox->addItem(type_name);
}

void SettingGuiWireup::onPortTypeChange( QString new_type )
{
    if( !valid )
        return;

    current = type_to_builder.value( new_type );
    setCurrentIndex( type_to_index.value( new_type ) );
}

void SettingGuiWireup::onChannelChange( QString new_channel )
{
    if( !valid )
        return;

    if( current ) {
        emit storeSettings( current->getSettings(), last_channel );
    }

    last_channel = new_channel.toInt();

    PortSettingsPointer settings =
            channels->getPortSettings( last_channel );

    if( !settings )
        return;

    QString type_name = settings->getTypeName();
    current = type_to_builder.value( type_name );

    if( !current )
        return;

    port_type_combobox->setCurrentText( type_name );
    current->setSettings( settings );

    int idx = type_to_index.value( type_name );
    setCurrentIndex( idx );
}

void SettingGuiWireup::setSettings(PortSettingsPointer settings)
{
    if( !valid )
        return;

    current->setSettings( settings );
}
