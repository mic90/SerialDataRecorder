#-------------------------------------------------
#
# Project created by QtCreator 2016-08-16T11:38:33
#
#-------------------------------------------------

QT       += core gui serialport widgets printsupport

CONFIG += c++14

TARGET = SerialDataRecorder
TEMPLATE = app
DESTDIR = $$PWD/../bin

include(external/qslog/QsLog.pri)

SOURCES += main.cpp\
        mainwindow.cpp \
    project/project.cpp \
    project/chart.cpp \
    serial/serialport.cpp \
    serial/serialportconfig.cpp \
    ui/window_project.cpp \
    ui/window_variable.cpp \
    ui/window_configuration.cpp \
    ui/chartwidget.cpp \
    ui/resizablescrollarea.cpp \
    external/qcustomplot/qcustomplot.cpp \
    ui/window_channels.cpp \
    ui/window_charts.cpp \
    project/channel.cpp \
    ui/widget_chartconfiguration.cpp \
    ui/window_chartconfiguration.cpp \
    parsers/dataparserfactory.cpp \
    parsers/asciidataparser.cpp \
    serial/serialthread.cpp \
    csv/csvdata.cpp \
    csv/csvexporter.cpp

HEADERS  += mainwindow.h \
    project/project.h \
    project/chart.h \
    serial/serialport.h \
    serial/serialportconfig.h \
    ui/window_project.h \
    ui/window_variable.h \
    ui/window_configuration.h \
    ui/chartwidget.h \
    ui/resizablescrollarea.h \
    external/qcustomplot/qcustomplot.h \
    ui/window_channels.h \
    ui/window_charts.h \
    project/channel.h \
    ui/widget_chartconfiguration.h \
    ui/window_chartconfiguration.h \
    parsers/dataparserfactory.h \
    parsers/dataparser.h \
    parsers/asciidataparser.h \
    serial/serialthread.h \
    csv/csvdata.h \
    csv/csvexporter.h

FORMS    += mainwindow.ui \
    window_project.ui \
    window_variable.ui \
    window_configuration.ui \
    chartwidget.ui \
    window_channels.ui \
    window_charts.ui \
    widget_chartconfiguration.ui \
    window_chartconfiguration.ui

RESOURCES += \
    res.qrc
