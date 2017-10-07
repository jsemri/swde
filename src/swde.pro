#-------------------------------------------------
#
# Project created by QtCreator 2017-10-05T12:00:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = swde
TEMPLATE = app


SOURCES +=   \
        mainwindow.cpp \
        main.cpp \
    canvas.cpp \
    flowchartitem.cpp \
    debug.cpp

HEADERS  += mainwindow.h \
   canvas.h \
   flowchartitem.h \
   debug.h

FORMS    += mainwindow.ui
