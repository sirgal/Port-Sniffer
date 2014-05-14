#ifndef SETTINGGUIWIREUP_H
#define SETTINGGUIWIREUP_H

#include <QObject>
#include <QComboBox>
#include <QFormLayout>
#include <QListWidget>
#include <QMap>

#include "ports/port_settings/port_settings.h"
#include "ports/gui_builders/port_guibuilder.h"
#include "channels/channel_factory.h"

class SettingGuiWireup : public QObject
{
    Q_OBJECT

    QComboBox *port_type_combobox;
    QFormLayout *port_settings_frame;
    QListWidget *channel_list;
    ChannelFactory *channels;

    QMap<QString, PortGuiBuilderPointer> type_to_builder;

    int last_channel = 0;
    PortGuiBuilderPointer current = nullptr;

    void setFrame();
public slots:
    void onPortTypeChange(QString new_type);
    void onChannelChange(QString new_channel);
    void setSettings(PortSettingsPointer settings);

signals:
    void storeSettings(PortSettingsPointer settings, int chan_idx);

public:
    SettingGuiWireup( QComboBox *port_type_combobox,
                      QFormLayout *port_settings_frame,
                      QListWidget *channel_list );

    void registerPortType( PortGuiBuilderPointer builder );
};

#endif // SETTINGGUIWIREUP_H
