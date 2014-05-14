#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QEvent>
#include <QDropEvent>
#include <QListWidget>
#include <QtSerialPort/QSerialPortInfo>
#include <QtAlgorithms>
#include <QColorDialog>
#include <QMessageBox>

#include <chrono>
#include <iostream>

#include <portdatasorter.h>
#include <drawdata.h>
#include <channels/channel_settings.h>

#include "parser/interface.h"
#include "channels/channel_factory.h"

#include "ports/gui_builders/comport_guilbuilder.h"
#include "ports/gui_builders/dummyport_guibuilder.h"
#include "ports/gui_builders/guibuilder_factory.h"
#include "dataholder.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
public slots:
    void scrollData(int dx );
    void labelResized();

    void addChannel();
    void openChannel( QListWidgetItem *list_item );
    void deleteChannel();
    void toggleChannel();
    void channelTypeChanged();

    void startSniffingButt();
    void startRetranslatingButt();

    void setChannelColor();
    void channelPortChanged( QString new_name );

    void dummyParseLineEditClicked();
    void parseEditorClosed();

    void showPreprocessed();
    bool setParser();
    void setParserAndReparse();
private:
    Ui::MainWindow *ui;
    DataHolder data_holder;
    ChannelFactory channel_factory;

    enum class States { Intermission, Sniffing, Retranslating };

    const int max_channels;
    int last_dx;
    int channels;
    int current_channel;
    States state;

    DrawData drawer;
    PortDataSorter sorter;

    QList<ChannelSettings> channel_settings;
    QList<int> deleted_channels;

    void disableInterface();
    void enableInterface();

    void startRetrans();
    void startSniffer();

    void stopSniffer();
    void stopRetrans();

    void addTestData();
};

#endif // MAINWINDOW_H
