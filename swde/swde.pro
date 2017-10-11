#-------------------------------------------------
#
# Project created by QtCreator 2017-10-05T12:00:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = flowchart_editor
TEMPLATE = app


SOURCES +=   \
        src/mainwindow.cpp \
        src/main.cpp \
    src/canvas.cpp \
    src/flowchartitem.cpp \
    src/textfield.cpp \
    src/line.cpp

HEADERS  += src/mainwindow.h \
   src/canvas.h \
   src/flowchartitem.h \
    src/textfield.h \
    src/line.h

RESOURCES += images.qrc

FORMS    += src/mainwindow.ui
