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
    gui_builders.append(
                std::dynamic_pointer_cast<PortGuiBuilder>(
                    std::make_shared<EmulatedPortGuiBuilder>()
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

    connect( ui->channelList,
             SIGNAL(currentTextChanged(QString)),
             this,
             SLOT(channelPortChanged(QString))
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

    connect( &sorter, SIGNAL(gotByte(timestamped_data)), &data_holder, SLOT(receiveByte(timestamped_data)) );

    addTestData();

    ui->expandedParserCont->hide();
    ui->preprocessedParseEditLabel->hide();
    ui->preprocessedParseEdit->hide();

    ui->portTypeComboBox->setEnabled( false );

    ui->channelColor->setAutoFillBackground( true );

    setLabelColor( 0, ui->channelColor );

    scrollData( last_dx );

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
    int chan_num = deleted_channels.isEmpty() ? channels++ : deleted_channels.takeFirst();

    channel_factory->addChannel( chan_num );

    QString chan_name = QString::number(chan_num );

    ui->channelList->addItem( chan_name );
    ui->channelList->setCurrentRow( ui->channelList->count() - 1 );
    ui->channelList->currentTextChanged( chan_name );

    ui->portTypeComboBox->setEnabled( true );
}

void MainWindow::deleteChannel()
{
    QListWidgetItem *item = ui->channelList->currentItem();

    if( !item )
        return;

    int chan_num = item->text().toInt();
    int row = ui->channelList->row( item );

    channel_factory->removeChannel( chan_num );
    ui->channelList->model()->removeRow( row );

    deleted_channels.append( chan_num );
    qSort( deleted_channels );

    // if no channels available - prevent port type change
    if( ui->channelList->model()->rowCount() == 0 )
        ui->portTypeComboBox->setEnabled( false );
}

void MainWindow::toggleChannel()
{
    int chan_num = getCurrentChanNum();

    if( chan_num < 0 )
        return;

    bool is_enabled = ui->channelEnable->isChecked();

    channel_factory->setEnabledChannel( chan_num, is_enabled );
    drawer.setEnabledChannel( chan_num, is_enabled );
    scrollData( last_dx );
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
    if( setParser() ) {
        data_holder.reparse();
        drawer.setData( data_holder.getParsed() );
        scrollData( last_dx );
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
    if( !data_holder.isEmpty() ) {
        QMessageBox::StandardButton choice = QMessageBox::question(
                    this,
                    "There's data there",
                    "Overwrite current data?",
                    QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
                    );

        switch( choice ) {
        case QMessageBox::Yes:
            data_holder.clear();
            break;
        case QMessageBox::No:
            break;
        case QMessageBox::Cancel:
            return;
        default:
            return;
        }
    }

    state = States::Sniffing;
    disableInterface();

    // stimulate settings save by emitting signal
    ui->channelList->currentTextChanged(
                    ui->channelList->currentItem()->text()
                );

    try {
        channel_factory->startAll();
    } catch( QString &error_string ) {
        QMessageBox::critical(
                    this,
                    "Failed to open port",
                    QString("And that's why:\n") + error_string
                    );
    }
}

void MainWindow::stopSniffer()
{
    state = States::Intermission;
    enableInterface();
    channel_factory->stopAll();
    drawer.setData( data_holder.getParsed() );
    scrollData( 0 );
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
        for( int j = 0; j < (i%5) + 1; j++ ) {
            channel_data_pair chan_data = { i % 10, i % 256 };
            timestamped_data data = { chan_data, i };
            data_holder.receiveByte( data );
        }
    }

    drawer.setData( data_holder.getUnparsed() );
}

int MainWindow::getCurrentChanNum()
{
    QListWidgetItem *item = ui->channelList->currentItem();

    if( !item )
        return -1;
    else
        return item->text().toInt();
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
    int chan_num = new_name.toInt();

    setLabelColor( chan_num, ui->channelColor );

    bool is_enabled = channel_factory->isChannelEnabled( chan_num );
    ui->channelEnable->setChecked( is_enabled );
}

void MainWindow::setLabelColor( int chan_num, QLabel *label )
{
    QColor color = channel_factory->getChannelColor( chan_num );
    QPalette palette = label->palette();
    palette.setColor( label->backgroundRole(), color );
    label->setPalette( palette );
}

void MainWindow::setChannelColor()
{
    QColorDialog color_dialog( this );
    int chan_num = getCurrentChanNum();

    if( chan_num < 0 )
        return;

    QColor initial = channel_factory->getChannelColor( chan_num );
    QColor next = color_dialog.getColor( initial, this );

    if( next == initial )
        return;

    channel_factory->setChannelColor( chan_num, next );
    setLabelColor( chan_num, ui->channelColor );
    drawer.setChannelColor( chan_num, next );
    scrollData( last_dx );
}
