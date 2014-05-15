#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    :
      QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->channelList->setDragEnabled( false );

    channel_factory = new ChannelFactory( &sorter );

    ui->portSettingsForm->registerFriends(
                ui->portTypeComboBox,
                ui->channelList,
                channel_factory
                );

    gui_builders.append(
                std::dynamic_pointer_cast<PortGuiBuilder>(
                    std::make_shared<DummyPortGuiBuilder>()
                    )
                );
    gui_builders.append(
                std::dynamic_pointer_cast<PortGuiBuilder>(
                    std::make_shared<ComPortGuiBuilder>()
                    )
                );

    foreach( auto builder, gui_builders ) {
        ui->portSettingsForm->registerPortType( builder );
    }

    connect( ui->channelList,
             SIGNAL(currentTextChanged(QString)),
             ui->portSettingsForm,
             SLOT(onChannelChange(QString))
             );

    connect( ui->portTypeComboBox,
             SIGNAL(currentTextChanged(QString)),
             ui->portSettingsForm,
             SLOT(onPortTypeChange(QString))
             );

    connect( ui->portSettingsForm,
             SIGNAL(storeSettings(PortSettingsPointer,int)),
             channel_factory,
             SLOT(storeSettings(PortSettingsPointer,int))
             );

    connect( ui->horizontalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(scrollData(int)));
    connect( ui->label,               SIGNAL(newSize(QSize)),    this, SLOT(labelResized()) );

    connect( ui->addChannel,          SIGNAL(clicked()),         this, SLOT(addChannel())   );
    connect( ui->deleteChannel,       SIGNAL(clicked()),         this, SLOT(deleteChannel()));

    connect( ui->channelEnable,       SIGNAL(toggled(bool)),     this, SLOT(toggleChannel()));
    connect( ui->channelSetColor,     SIGNAL(clicked()),         this, SLOT(setChannelColor()) );

    connect( ui->snifferStart,        SIGNAL(clicked()),         this, SLOT(startSniffingButt()) );
    connect( ui->retrStart,           SIGNAL(clicked()),         this, SLOT(startRetranslatingButt()) );

    connect( ui->parserEditDummy,     SIGNAL(gotFocus()),        this, SLOT(dummyParseLineEditClicked()) );
    connect( ui->showPreprocessedButt,SIGNAL(clicked()),         this, SLOT(showPreprocessed()) );
    connect( ui->parserSetButt,       SIGNAL(clicked()),         this, SLOT(setParser()) );
    connect( ui->parserSetReparseButt,SIGNAL(clicked()),         this, SLOT(setParserAndReparse()) );
    connect( ui->hideParseEditButt,   SIGNAL(clicked()),         this, SLOT(parseEditorClosed()) );

    addTestData();

    ui->expandedParserCont->hide();

    scrollData( last_dx );

    ui->preprocessedParseEditLabel->hide();
    ui->preprocessedParseEdit->hide();
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

void MainWindow::addChannel()
{
    int channel_number = deleted_channels.isEmpty() ? ++channels : deleted_channels.takeFirst();

    channel_factory->addChannel( channel_number );

    QString chan_name = QString::number(channel_number);

    ui->channelList->addItem( chan_name );
    ui->channelList->setCurrentRow( ui->channelList->count()  );
    ui->channelList->currentTextChanged( chan_name );
}

void MainWindow::deleteChannel()
{
    auto item = ui->channelList->currentItem();
    int chan_num = item->text().toInt();

    channel_factory->removeChannel( chan_num );
    ui->channelList->removeItemWidget( item );
    deleted_channels.append( chan_num );
}

void MainWindow::toggleChannel()
{
    int chan_num = ui->channelList->currentItem()->text().toInt();
    bool is_enabled = ui->channelEnable->isChecked();

    channel_factory->setEnabledChannel( chan_num, is_enabled );
    drawer.setEnabledChannel( chan_num, is_enabled );
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

bool MainWindow::setParser()
{
    QString parse_line = ui->parserEdit->document()->toPlainText();

    try {
        data_holder.setParserProgram( parse_line );
        ui->preprocessedParseEdit->setText( data_holder.getPreprocessedString() );
        return true;
    } catch( ExceptionWithPos &e ) {
        ui->preprocessedParseEdit->setText( data_holder.getPreprocessedString() );

        QMessageBox::warning( this, "ERROR", QString(e.what()) + ' ' + QString::number(e.position)  );

        QTextCursor cursor = ui->preprocessedParseEdit->textCursor();
        cursor.setPosition( e.position );
        cursor.select( QTextCursor::WordUnderCursor );

        ui->preprocessedParseEdit->setTextCursor( cursor );
        ui->preprocessedParseEdit->setFocus();

        return false;
    }
}

void MainWindow::setParserAndReparse()
{
    if( setParser() )
        data_holder.reparse();
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

void MainWindow::channelPortChanged( QString new_name )
{
}

void MainWindow::setChannelColor()
{
}
