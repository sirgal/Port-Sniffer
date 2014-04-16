#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    gui_factory(ui->portSettingsForm),
    channel_factory(&sorter),
    max_channels(16)
{
    ui->setupUi(this);
    last_dx = 0;
    current_channel = 0;
    state = States::Intermission;
    available_speeds = { "2400",  "4800",   "9600",   "14400",
                         "19200", "28800",  "38400",  "56000",
                         "57600", "115200", "128000", "256000" };

    qsrand( QTime::currentTime().msec() );

    channels = 0;

    ui->channelSettings->hide();

    ui->channelList->setDragEnabled( false );

    connect( ui->channelList,         SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(openChannel(QListWidgetItem*)) );
    connect( ui->horizontalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(scrollData(int)));
    connect( ui->label,               SIGNAL(newSize(QSize)),    this, SLOT(labelResized()) );
    connect( ui->channelEnable,       SIGNAL(toggled(bool)),     this, SLOT(toggleChannel()));
    connect( ui->addChannel,          SIGNAL(clicked()),         this, SLOT(addChannel())   );
    connect( ui->deleteChannel,       SIGNAL(clicked()),         this, SLOT(deleteChannel()));
    connect( ui->channelSetColor,     SIGNAL(clicked()),         this, SLOT(setChannelColor()) );
    connect( ui->snifferStart,        SIGNAL(clicked()),         this, SLOT(startSniffingButt()) );
    connect( ui->retrStart,           SIGNAL(clicked()),         this, SLOT(startRetranslatingButt()) );
    connect( ui->portTypeComboBox,    SIGNAL(activated(QString)),this, SLOT(channelPortChanged(QString)));
    connect( ui->parserEditDummy,     SIGNAL(gotFocus()),        this, SLOT(dummyParseLineEditClicked()) );
    connect( ui->showPreprocessedButt,SIGNAL(clicked()),         this, SLOT(showPreprocessed()) );
    connect( ui->parserSetButt,       SIGNAL(clicked()),         this, SLOT(setParser()) );
    connect( ui->hideParseEditButt,   SIGNAL(clicked()),         this, SLOT(parseEditorClosed()) );

    addTestData();

    ui->expandedParserCont->hide();

    scrollData( last_dx );

    ui->preprocessedParseEditLabel->hide();
    ui->preprocessedParseEdit->hide();

    auto ptr = std::dynamic_pointer_cast<PortGuiBuilder>(std::make_shared<ComPortGuiBuilder>());
    gui_factory.registerPortType( ptr );

    ui->portTypeComboBox->addItems( gui_factory.getAvailableTypes() );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::scrollData( int dx )
{
    last_dx = dx;
    QPixmap *pixmap = new QPixmap( ui->label->size() );
    drawer.createImage( dx, pixmap );
    ui->label->setPixmap( *pixmap );
    delete pixmap;
}

void MainWindow::labelResized()
{
    scrollData(last_dx);
}

void MainWindow::openChannel(QListWidgetItem* list_item)
{
    ChannelPointer channel = channel_factory.findChannel( list_item->text().toInt() );

}

void MainWindow::addChannel()
{
    ui->channelSettings->show();

    gui_factory.setType( ui->portTypeComboBox->currentText() );
}

void MainWindow::deleteChannel()
{
}

void MainWindow::toggleChannel()
{
}

void MainWindow::dummyParseLineEditClicked()
{
    ui->minimizedParserCont->hide();
    ui->expandedParserCont->show();
    ui->parserEdit->setFocus();
}

void MainWindow::parseEditorClosed()
{
    ui->expandedParserCont->hide();
    ui->minimizedParserCont->show();
    ui->parserEditDummy->setText( ui->parserEdit->toPlainText() );
}

void MainWindow::showPreprocessed()
{
    bool hidden = ui->preprocessedParseEdit->isHidden();

    if( hidden )
        ui->showPreprocessedButt->setText("Hide preprocessed");
    else
        ui->showPreprocessedButt->setText("Show preprocessed");

    ui->preprocessedParseEdit->setHidden(!hidden);
    ui->preprocessedParseEditLabel->setHidden(!hidden);
}

void MainWindow::setParser()
{
    QString parse_line = ui->parserEdit->document()->toPlainText();

    try {
        parser.setParserString( parse_line.toStdString() );
        ui->preprocessedParseEdit->setText( QString(parser.getPreprocessed().data()) );
    } catch( ExceptionWithPos &e ) {
        ui->preprocessedParseEdit->setText( QString(parser.getPreprocessed().data()) );

        QMessageBox::warning( this, "ERROR", QString(e.what()) + ' ' + QString::number(e.position)  );

        QTextCursor cursor = ui->preprocessedParseEdit->textCursor();
        cursor.setPosition( e.position );
        cursor.select( QTextCursor::WordUnderCursor );

        ui->preprocessedParseEdit->setTextCursor( cursor );

        ui->preprocessedParseEdit->setFocus();
    }
}

void MainWindow::startSniffingButt()
{
    switch( state ) {
    case States::Intermission:
        startSniffer();
        break;
    case States::Sniffing:
        stopSniffer();
        break;
    case States::Retranslating:
        stopRetrans();
        break;
    }
}

void MainWindow::startRetranslatingButt()
{
    switch( state ) {
    case States::Intermission:
        startRetrans();
        break;
    case States::Sniffing:
        stopSniffer();
        break;
    case States::Retranslating:
        stopRetrans();
        break;
    }
}

void MainWindow::startRetrans()
{
    state = States::Retranslating;
    disableInterface();
}

void MainWindow::startSniffer()
{
    state = States::Sniffing;
    disableInterface();
}

void MainWindow::stopSniffer()
{
    state = States::Intermission;
    enableInterface();
}

void MainWindow::stopRetrans()
{
    state = States::Intermission;
    enableInterface();
}

void MainWindow::addTestData()
{
    drawer.clear();
    drawer.setChannelCount(10);

    for( int i = 0; i < 65535; i++ ) {
        for( int j = 0; j < (i%5) + 1; j++ )
            drawer.appendData( i % 10, i % 256 );
    }
}

void MainWindow::disableInterface()
{
    const QList<QWidget*> to_disable =
    {
        ui->retransSettings,
        ui->channelSettings,
        ui->deleteChannel,
        ui->channelList,
        ui->addChannel,
        ui->parserEdit,
        ui->parserEditDummy
    };

    foreach( QWidget* widget, to_disable )
        widget->setDisabled(true);

    ui->snifferStart->setText("STOP");
    ui->retrStart->setText("STOP");
}

void MainWindow::enableInterface()
{
    const QList<QWidget*> to_enable =
    {
        ui->retransSettings,
        ui->channelSettings,
        ui->deleteChannel,
        ui->channelList,
        ui->addChannel,
        ui->parserEdit,
        ui->parserEditDummy
    };

    foreach( QWidget* widget, to_enable )
        widget->setDisabled(false);

    ui->retrStart->setText("Start Retranslator");
    ui->snifferStart->setText("Start Sniffer");
}

QStringList MainWindow::availablePorts()
{
    QSerialPortInfo info;
    QStringList output;
    foreach( QSerialPortInfo port, info.availablePorts() ) {
        if( !occupied_ports.contains( port.portName() ) )
            output.append(port.portName());
    }
    return output;
}

void MainWindow::setColor( QWidget *widget, QColor color )
{
    QPalette palette = widget->palette();
    palette.setColor( QPalette::Background, color );
    widget->setPalette( palette );
    widget->setAutoFillBackground( true );
}

void MainWindow::channelPortChanged( QString new_name )
{
}

void MainWindow::setChannelColor()
{
}
