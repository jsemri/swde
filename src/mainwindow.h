#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "diagramnode.h"

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
    QButtonGroup *node_buttons;
    QButtonGroup *background_buttons;

    QAction *exit_action;
    QAction *about_action;
    QAction *undo_action;
    QAction *delete_action;

    Ui::MainWindow *ui;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void create_actions();
    void create_toolbox();
    void create_menus();

    static QWidget *widget_layout(QLayout *layout);

    QWidget *
    create_bg_cell_widget(const QString &text, const QString &image);
    QWidget *
    create_cell_widget(const QString &text, DiagramNode::DiagramNodeType type);

private slots:
    void about();
    void undo();
    void delete_item();
    void node_button_clicked(int id);
};

#endif // MAINWINDOW_H
