#include <QApplication>
#include "mainwindow.h"
#include "debug.h"

// goals:
//      don't let items go beyond scene border
//      style
//      arrows and lines
//      text insertion
//      advanced oprations: export, undo, etc.

int main(int argc, char *argv[])
{
    Debug::set_dbg_level(Debug::low);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
