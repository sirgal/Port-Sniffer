#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QEvent>
#include <QDropEvent>
#include <QListWidget>
#include <QtSerialPort/QSerialPortInfo>
#include <QTime>
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
    void openChannel(QListWidgetItem *list_item);
    void deleteChannel();
    void toggleChannel();

    void startSniffingButt();
    void startRetranslatingButt();

    void setChannelColor();
    void channelPortChanged( QString new_name );

    void dummyParseLineEditClicked();
    void parseEditorClosed();

    void showPreprocessed();
    void setParser();
private:
    Ui::MainWindow *ui;
    ParserInterface parser;
    ComPortGuiBuilder *guibuilder;
    ChannelFactory chan_factory;

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
    QStringList available_speeds;
    QSet<QString> occupied_ports;

    void disableInterface();
    void enableInterface();

    void startRetrans();
    void startSniffer();

    void stopSniffer();
    void stopRetrans();

    void addTestData();

    QStringList availablePorts();
    void setColor( QWidget *widget, QColor color );
};

#endif // MAINWINDOW_H
