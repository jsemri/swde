#include <QApplication>
#include "mainwindow.h"


// goals:
//      toolbar: brush, border |text size, bold, italic,
//               underline,| to front, to back, scale|
//      export, undo, save, open, new

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
