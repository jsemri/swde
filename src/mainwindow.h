#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Canvas;
class QGraphicsView;
class QHBoxLayout;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QHBoxLayout *layout;
    QGraphicsView *view;
    Canvas *canvas;

    Ui::MainWindow *ui;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void setup_actions();
    void setup_toolbox();
    void setup_menus();
};

#endif // MAINWINDOW_H
