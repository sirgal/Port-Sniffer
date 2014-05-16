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
    paramstringprocessor.h \
    selfresizinglabel.h \
    lineeditwithfocussignals.h \
    dataholder.h \
    settingguiwireup.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/build/build-parser-lib/release/ -llibparser
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/build/build-parser-lib/debug/ -llibparser
else:unix: LIBS += -L$$PWD/build/build-parser-lib/ -llibparser
