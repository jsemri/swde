#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "flowpolygon.h"
#include "flowitem.h"

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
class QGridLayout;
/*
namespace Ui {
class MainWindow;
}*/

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void createActions();
    void createToolbox();
    void createMenus();


    void createItemButton(QGridLayout *gLayout, FlowItem::Type type, QString statusTip);
    void keyPressEvent(QKeyEvent *event) override;

    static QWidget *widgetLayout(QLayout *layout);
    QHBoxLayout *layout;
    QGraphicsView *view;
    Canvas *canvas;

    // top menus
    QMenu *fileMenu;
    QMenu *editMenu;

    // bottom bar
    QToolBox *toolbox;
    QButtonGroup *itemButtons;
    QButtonGroup *backgroundButtons;

    // top bar
    QToolBar *toolbar;
    QToolBar *editToolbar;
    QToolBar *textToolbar;
    QToolBar *itemToolbar;

    // actions
    QAction *exitAction;
    //Ui::MainWindow *ui;


private slots:
    void itemButtonClicked(int id);
    void textInserted(QGraphicsTextItem *item);
    void itemInserted(FlowPolygon *item);
    void itemSelected(QGraphicsItem *item);
    void arrowInserted();
};

#endif // MAINWINDOW_H
