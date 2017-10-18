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

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void createActions();
    void createToolbox();
    void createToolbars();
    void createMenus();
    QIcon createColorIcon(QColor color);
    QIcon createBorderIcon(QColor color, int width);
    QMenu *createColorMenu(const char *slot, QColor defaultColor);

    void keyPressEvent(QKeyEvent *event) override;
    void createItemButton(
            QGridLayout *gLayout, FlowItem::Type type, QString statusTip);

    static QWidget *widgetLayout(QLayout *layout);

    QHBoxLayout *layout;
    QGraphicsView *view;
    Canvas *canvas;
    QGraphicsItem *itemCopy;

    // top menus
    QMenu *fileMenu;
    QMenu *editMenu;

    // bottom bar
    QToolBox *toolbox;
    QButtonGroup *itemButtons;

    // top bar
    QToolBar *editToolbar;
    QToolBar *textToolbar;
    QToolBar *itemToolbar;
    QToolBar *aspectToolbar;

    QToolButton *colorButton;
    QToolButton *borderColorButton;
    QComboBox *borderWidthCombo;

    // actions
    QAction *exitAction;
    QAction *deleteAction;
    QAction *copyAction;
    QAction *pasteAction;

    QAction *putFrontAction;
    QAction *putBackAction;

    QAction *changeColorAction;
    QAction *changeBorderColorAction;
    QAction *changeBorderWidthAction;


private slots:
    void itemButtonClicked(int id);
    void textInserted(QGraphicsTextItem *item);
    void itemInserted(FlowPolygon *item);
    void itemSelected(QGraphicsItem *item);
    void arrowInserted();
    void deleteItem();
    void putFront();
    void putBack();
    void copy();
    void paste();
    void scaleChanged(const QString &scale);
    void changeColor();
    void colorButtonTriggered();
    void changeBorderColor();
};

#endif // MAINWINDOW_H
