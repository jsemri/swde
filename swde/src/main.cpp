#include <QApplication>
#include "mainwindow.h"

// TODO
// undo
// rect select
// right click

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
