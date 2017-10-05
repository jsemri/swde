#-------------------------------------------------
#
# Project created by QtCreator 2017-10-05T12:00:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = swde
TEMPLATE = app


SOURCES +=   src/state.cpp \
        src/mainwindow.cpp \
        src/main.cpp

HEADERS  += src/mainwindow.h \
    src/state.h

FORMS    += src/mainwindow.ui
