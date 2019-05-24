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
#include "ports/gui_builders/emulatedport_guibuilder.h"

#include "settingguiwireup.h"
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
    void deleteChannel();
    void toggleChannel();

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
    ChannelFactory *channel_factory;

    enum class States { Intermission, Sniffing, Retranslating };

    const int max_channels = 16;
    int last_dx = 0;
    int channels = 0;
    States state = States::Intermission;

    DrawData drawer;
    PortDataSorter sorter;

    QList<ChannelSettings> channel_settings;
    QList<int> deleted_channels;
    QList<PortGuiBuilderPointer> gui_builders;

    void disableInterface();
    void enableInterface();

    void startRetrans();
    void startSniffer();

    void stopSniffer();
    void stopRetrans();

    void addTestData();
    void clearData();

    int getCurrentChanNum();
    void setLabelColor(int chan_num, QLabel *label);
};

#endif // MAINWINDOW_H
