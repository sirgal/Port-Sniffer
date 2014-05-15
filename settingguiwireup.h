#ifndef SETTINGGUIWIREUP_H
#define SETTINGGUIWIREUP_H

#include <QStackedWidget>
#include <QComboBox>
#include <QStackedLayout>
#include <QListWidget>
#include <QMap>

#include "ports/port_settings/port_settings.h"
#include "ports/gui_builders/port_guibuilder.h"
#include "channels/channel_factory.h"

class SettingGuiWireup : public QStackedWidget
{
    Q_OBJECT

    QComboBox *port_type_combobox;
    QListWidget *channel_list;
    ChannelFactory *channels;

    QMap<QString, PortGuiBuilderPointer> type_to_builder;
    QMap<QString, int> type_to_index;

    bool valid = false;
    int last_channel = -1;
    PortGuiBuilderPointer current = nullptr;

public slots:
    void onPortTypeChange( QString new_type );
    void onChannelChange( QString new_channel );

signals:
    void storeSettings( PortSettingsPointer settings, int chan_idx );

public:
    SettingGuiWireup( QWidget *parent );

    void registerFriends( QComboBox *port_type_combobox,
                          QListWidget *channel_list,
                          ChannelFactory *channels
                          );

    void registerPortType( PortGuiBuilderPointer builder );
    void setSettings( PortSettingsPointer settings );
};

#endif // SETTINGGUIWIREUP_H
