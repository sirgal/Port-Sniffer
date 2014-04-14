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
    ports/port_settings/comport_settings.cpp \
    ports/comport.cpp \
    ports/gui_builders/comport_guilbuilder.cpp \
    ports/gui_builders/port_guibuilder.cpp \
    ports/port_factory.cpp \
    channels/channel.cpp \
    channels/channel_factory.cpp \
    channels/channel_settings.cpp \
    ports/port_settings/port_settings.cpp

HEADERS  += mainwindow.h \
    drawdata.h \
    portdatasorter.h \
    paramstringprocessor.h \
    selfresizinglabel.h \
    lineeditwithfocussignals.h \
    dataholder.h \
    ports/port.h \
    ports/comport.h \
    ports/emulatedport.h \
    channels/channel.h \
    ports/port_settings/comport_settings.h \
    ports/port_settings/emulatedport_settings.h \
    ports/gui_builders/comport_guilbuilder.h \
    ports/gui_builders/emulatedport_guibuilder.h \
    ports/gui_builders/port_guibuilder.h \
    ports/port_settings/port_settings.h \
    channels/channel_factory.h \
    ports/port_factory.h \
    channels/channel_settings.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build/build-port_sniffer_parser-Desktop_Qt_5_1_1_MinGW_32bit-Release/release/ -llibport_sniffer_parser
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build/build-port_sniffer_parser-Desktop_Qt_5_1_1_MinGW_32bit-Debug/debug/ -llibport_sniffer_parser

INCLUDEPATH += $$PWD/../port_sniffer_parser
DEPENDPATH += $$PWD/../port_sniffer_parser
