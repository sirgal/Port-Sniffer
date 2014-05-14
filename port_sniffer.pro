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
    ports/port_settings/comport_settings.cpp \
    ports/comport.cpp \
    ports/gui_builders/comport_guilbuilder.cpp \
    ports/gui_builders/port_guibuilder.cpp \
    channels/channel.cpp \
    channels/channel_settings.cpp \
    ports/port_settings/port_settings.cpp \
    ports/gui_builders/emulatedport_guibuilder.cpp \
    ports/port_settings/dummyport_settings.cpp \
    channels/channel_factory.cpp \
    settingguiwireup.cpp

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
    channels/channel_settings.h \
    parser/testcases.h \
    parser/interface.h \
    parser/actions/triggeraction.h \
    parser/actions/receiveaction.h \
    parser/actions/dropaction.h \
    parser/actions/countingaction.h \
    parser/actions/action_enums.h \
    parser/actions/action.h \
    parser/algorithms/streamkmp.h \
    parser/algorithms/parserexceptions.h \
    parser/algorithms/parser.h \
    parser/algorithms/algorithms_header.h \
    parser/containers/strongaction.h \
    parser/containers/cycliccontainer.h \
    parser/containers/container.h \
    parser/containers/anychannelaction.h \
    parser/containers/actioncontainer.h \
    ports/gui_builders/guibuilder_factory.h \
    ports/port_settings/dummyport_settings.h \
    ports/dummyport.h \
    ports/gui_builders/dummyport_guibuilder.h \
    settingguiwireup.h

FORMS    += mainwindow.ui
