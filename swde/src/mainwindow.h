#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "flowchartitem.h"

class Canvas;
class QGraphicsView;
class QHBoxLayout;
class QToolBox;
class QButtonGroup;
class QComboBox;
class QToolButton;
class QFontComboBox;
class QGraphicsTextItem;
class QGraphicsItem;

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

    // top menus
    QMenu *file_menu;
    QMenu *edit_menu;

    // bottom bar
    QToolBox *toolbox;
    QButtonGroup *item_buttons;
    QButtonGroup *background_buttons;

    // top bar
    QToolBar *toolbar;
    QToolBar *edit_toolbar;
    QToolBar *text_toolbar;
    QToolBar *item_toolbar;

    // actions
    QAction *exit_action;
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
    create_cell_widget(const QString &text,
                       FlowChartItem::FlowChartItemType type);

    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void item_button_clicked(int id);
    void textInserted(QGraphicsTextItem *item);
    void itemSelected(QGraphicsItem *item);
};

#endif // MAINWINDOW_H
