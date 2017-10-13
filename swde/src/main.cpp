#include <QApplication>
#include "mainwindow.h"


// goals:
//      item mouse resizing
//      line transformation
//      scene borders
//      toolbar: delete, copy, paste, color, border, text size, bold, italic, underline,
//               gradient, to front, to back
//      export, undo, save, load

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
