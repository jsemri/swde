#include <QApplication>
#include "mainwindow.h"


// goals:
//      toolbar:
//          <!> text size, bold, italic, underline (text)
//      new, save, open, export, very simple undo

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
