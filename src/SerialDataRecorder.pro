#-------------------------------------------------
#
# Project created by QtCreator 2016-08-16T11:38:33
#
#-------------------------------------------------

QT       += core gui serialport widgets printsupport

CONFIG += c++11

TARGET = SerialDataRecorder
TEMPLATE = app
DESTDIR = $$PWD/../bin

include(external/qslog/QsLog.pri)

SOURCES += main.cpp\
        mainwindow.cpp \
    project/project.cpp \
    serial/serialportconfig.cpp \
    project/chart.cpp \
    views/multiwindowview.cpp \
    project/projectmanager.cpp \
    views/variablesview.cpp \
    ui/window_project.cpp \
    ui/window_variable.cpp \
    ui/window_configuration.cpp \
    external/qcustomplot/qcustomplot.cpp \
    project/variable.cpp \
    parsers/mysensorsdataparser.cpp \
    serial/serialport.cpp \
    ui/variableitem.cpp \
    parsers/parsersmanager.cpp \
    parsers/sdrdataparser.cpp \
    ui/chartwidget.cpp

HEADERS  += mainwindow.h \
    project/project.h \
    serial/serialportconfig.h \
    project/chart.h \
    views/multiwindowview.h \
    project/projectmanager.h \
    views/variablesview.h \
    plugins/idataparser.h \
    ui/window_project.h \
    ui/window_variable.h \
    ui/window_configuration.h \
    external/qcustomplot/qcustomplot.h \
    project/variable.h \
    parsers/mysensorsdataparser.h \
    serial/serialport.h \
    ui/variableitem.h \
    parsers/parsersmanager.h \
    parsers/sdrdataparser.h \
    ui/chartwidget.h

FORMS    += mainwindow.ui \
    window_project.ui \
    window_variable.ui \
    window_configuration.ui \
    chartwidget.ui

RESOURCES += \
    res.qrc
