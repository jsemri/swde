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
        src/mainwindow.cpp \
        src/main.cpp \
    src/canvas.cpp \
    src/component.cpp

HEADERS  += src/mainwindow.h \
    src/canvas.h \
    src/component.h

FORMS    += src/mainwindow.ui
