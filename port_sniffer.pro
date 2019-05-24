#-------------------------------------------------
#
# Project created by QtCreator 2013-08-28T12:03:51
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = port_sniffer
TEMPLATE = app
CONFIG += c++1z

include(parser/parser.pri)
include(ports/ports.pri)
include(channels/channels.pri)

SOURCES += main.cpp\
    mainwindow.cpp \
    drawdata.cpp \
    portdatasorter.cpp \
    settingguiwireup.cpp

HEADERS  += mainwindow.h \
    drawdata.h \
    portdatasorter.h \
    selfresizinglabel.h \
    lineeditwithfocussignals.h \
    dataholder.h \
    settingguiwireup.h

FORMS    += mainwindow.ui
