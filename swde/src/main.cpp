#include <QApplication>
#include "mainwindow.h"


// goals:
//      don't let items go beyond scene border
//      style
//      arrows and lines
//      text insertion
//      advanced oprations: export, undo, etc.

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
