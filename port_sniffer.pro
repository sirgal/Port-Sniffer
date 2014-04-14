#-------------------------------------------------
#
# Project created by QtCreator 2013-08-28T12:03:51
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = port_sniffer
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    drawdata.cpp \
    portdatasorter.cpp \
    paramstringprocessor.cpp \
    lineeditwithfocussignals.cpp \
    ports/port_settings/comport_settings.cpp \
    ports/comport.cpp

HEADERS  += mainwindow.h \
    drawdata.h \
    portdatasorter.h \
    paramstringprocessor.h \
    selfresizinglabel.h \
    channels/channelsettings.h \
    lineeditwithfocussignals.h \
    dataholder.h \
    ports/port.h \
    ports/comport.h \
    ports/emulatedport.h \
    channels/channel.h \
    channels/channelfactory.h \
    ports/portfactory.h \
    ports/port_settings/portsettings.h \
    ports/port_settings/comport_settings.h \
    ports/port_settings/emulatedport_settings.h \
    ports/gui_builders/comportguilbuilder.h \
    ports/gui_builders/emulatedportguibuilder.h \
    ports/gui_builders/portguibuilder.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build/build-port_sniffer_parser-Desktop_Qt_5_1_1_MinGW_32bit-Release/release/ -llibport_sniffer_parser
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build/build-port_sniffer_parser-Desktop_Qt_5_1_1_MinGW_32bit-Debug/debug/ -llibport_sniffer_parser

INCLUDEPATH += $$PWD/../port_sniffer_parser
DEPENDPATH += $$PWD/../port_sniffer_parser
