#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "component.h"

class Canvas;
class QGraphicsView;
class QHBoxLayout;
class QToolBox;
class QButtonGroup;

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

    QMenu *file_menu;
    QMenu *edit_menu;
    QMenu *about_menu;

    QToolBox *toolbox;
    QToolBar *toolbar;
    QButtonGroup *button_gr;
    QButtonGroup *bg_button_gr;

    Ui::MainWindow *ui;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void create_actions();
    void create_toolbox();
    void create_menus();

    QWidget*
    create_bg_cell_widget(const QString &text, const QString &image);
    QWidget*
    create_cell_widget(const QString &text, Component::ComponentType type);

private slots:
    void about();
    void undo();
};

#endif // MAINWINDOW_H
