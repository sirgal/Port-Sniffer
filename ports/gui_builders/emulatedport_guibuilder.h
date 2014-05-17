#ifndef EMULATEDPORTGUIBUILDER_H
#define EMULATEDPORTGUIBUILDER_H

#include <QLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QStandardPaths>

#include "ports/gui_builders/port_guibuilder.h"
#include "ports/port_settings/emulatedport_settings.h"

class EmulatedPortGuiBuilder: public PortGuiBuilder
{
    Q_OBJECT

    EmulatedPortSettings shared_settings;
    PortSettingsPointer settings_pointer;

    QHBoxLayout path_and_button;
    QLineEdit file_path;
    QPushButton edit_path_button;

public slots:
    void changeFilePath()
    {
        QString folder;

        if( !file_path.text().isEmpty() )
            folder = file_path.text();
        else
            folder = QStandardPaths::standardLocations(
                            QStandardPaths::DocumentsLocation
                        ).first();

        QString new_file_path = QFileDialog::getOpenFileName(
                        &file_path,
                        QString("Select file"),
                        folder
                    );

        file_path.setText( new_file_path );
    }

public:
    EmulatedPortGuiBuilder()
    {
        settings_pointer = std::shared_ptr<PortSettings>(
                    dynamic_cast<PortSettings*>( &shared_settings )
                    );

        edit_path_button.setText( "..." );
        path_and_button.addWidget( &file_path );
        path_and_button.addWidget( &edit_path_button );

        connect( &edit_path_button, SIGNAL(clicked()), this, SLOT(changeFilePath()) );
    }

    virtual ~EmulatedPortGuiBuilder()
    {
        disconnect( &edit_path_button, SIGNAL(clicked()), this, SLOT(changeFilePath()) );
    }

    // we do nothing because settings are shared across ports
    // so everything is already set
    virtual void setSettings( PortSettings &settings )
    { Q_UNUSED(settings); }
    virtual void setSettings( PortSettingsPointer &settings )
    { Q_UNUSED(settings); }

    virtual PortSettingsPointer getSettings()
    {
        shared_settings.setFilePath( file_path.text() );
        return settings_pointer;
    }

    virtual void buildForm( QFormLayout *layout )
    {
        layout->addRow( "File path:", &path_and_button );
    }
    virtual QString getTypeName()
    { return EmulatedPortSettings().getTypeName(); }
};
#endif // EMULATEDPORTGUIBUILDER_H
