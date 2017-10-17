#include <QGraphicsView>
#include <QHBoxLayout>
#include <QToolBox>
#include <QToolButton>
#include <QLabel>
#include <QDebug>
#include <QPushButton>
#include <QComboBox>
#include <QFontComboBox>
#include <QToolBar>
#include <QKeyEvent>
#include <QDesktopWidget>
#include <QAction>
#include <QButtonGroup>
#include <QMenuBar>
#include <QMenu>

#include <algorithm>
#include <cassert>

#include "flowitem.h"
#include "mainwindow.h"
#include "canvas.h"
#include "flowpolygon.h"
#include "flowline.h"
#include "textfield.h"

#define ICON_X 80
#define ICON_Y 60

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), itemCopy{0}
{
    createActions();
    createMenus();
    createToolbox();

    canvas = new Canvas(editMenu, this);
    connect(canvas, SIGNAL(textInserted(QGraphicsTextItem*)), this,
            SLOT(textInserted(QGraphicsTextItem*)));
    connect(canvas, SIGNAL(itemInserted(FlowPolygon*)), this,
            SLOT(itemInserted(FlowPolygon*)));
    connect(canvas, SIGNAL(itemSelected(QGraphicsItem*)), this,
            SLOT(itemSelected(QGraphicsItem*)));
    connect(canvas, SIGNAL(arrowInserted(void)), this,
            SLOT(arrowInserted(void)));

    view = new QGraphicsView(canvas);
    view->setSceneRect(0, 0, 1000, 1000);
    view->fitInView(0, 0, 500, 400, Qt::KeepAspectRatio);
    view->setDragMode(QGraphicsView::NoDrag);
    view->setCacheMode(QGraphicsView::CacheBackground);
    //view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    view->setEnabled(true);

    createToolbars();
    layout = new QHBoxLayout();
    layout->addWidget(toolbox);
    layout->addWidget(view);
    setCentralWidget(widgetLayout(layout));
    setWindowIcon(QIcon(":/images/swde.png"));
    setWindowTitle("Simple Workflow Diagram Editor");
    setMinimumSize(600, 400);
    //setGeometry(qApp->desktop()->availableGeometry());
}

MainWindow::~MainWindow()
{
    if (itemCopy) {
        delete itemCopy;
    }
}

void MainWindow::createActions()
{
    exitAction = new QAction(tr("E&xit"), this);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    deleteAction = new QAction(QIcon(":/images/delete.png"), tr("Delete"),
                               this);
    deleteAction->setShortcut(tr("Delete"));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteItem()));

    putFrontAction = new QAction(QIcon(":/images/tofront.png"),
                                 tr("PutF ront"), this);
    connect(putFrontAction, SIGNAL(triggered()), this, SLOT(putFront()));

    putBackAction = new QAction(QIcon(":/images/toback.png"), tr("Put Back"),
                                this);
    connect(putBackAction, SIGNAL(triggered()), this, SLOT(putBack()));

    copyAction = new QAction(QIcon(":/images/copy.png"), tr("Copy"), this);
    copyAction->setShortcut(tr("Ctrl+c"));
    connect(copyAction, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAction = new QAction(QIcon(":/images/paste.png"), tr("Paste"), this);
    pasteAction->setShortcut(tr("Ctrl+v"));
    connect(pasteAction, SIGNAL(triggered()), this, SLOT(paste()));
    pasteAction->setChecked(false);
}

// create buttons on the right
// buttons manage which diagram element will be inserted
void MainWindow::createToolbox()
{
    // group of buttons which manage diagram elements
    itemButtons = new QButtonGroup(this);
    itemButtons->setExclusive(false);
    connect(itemButtons, SIGNAL(buttonClicked(int)), this,
            SLOT(itemButtonClicked(int)));

    QGridLayout *tbLayout = new QGridLayout;
    QMapIterator<FlowItem::Type, QString> it(FlowItem::tips);
    while (it.hasNext()) {
        it.next();
        createItemButton(tbLayout, it.key(), it.value());
    }
    //tbLayout->setColumnStretch(2, 10);
    tbLayout->setColumnMinimumWidth(0, 1.678 * ICON_X);
    QWidget *widget = widgetLayout(tbLayout);

    toolbox = new QToolBox();
    toolbox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,
                                       QSizePolicy::Ignored));
    toolbox->setMinimumWidth(widget->sizeHint().width());
    toolbox->addItem(widget, tr("afsd"));
    QColor col(3, 146, 13);
    toolbox->setPalette(QPalette(col));
    toolbox->setFrameShape(QFrame::WinPanel);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(exitAction);
}

void MainWindow::createToolbars() {
    editToolbar = addToolBar(tr("Edit Item"));
    editToolbar->addAction(deleteAction);
    editToolbar->addAction(copyAction);
    editToolbar->addAction(pasteAction);

    aspectToolbar = addToolBar(tr("Aspect"));
    aspectToolbar->addAction(putFrontAction);
    aspectToolbar->addAction(putBackAction);
}

QWidget *MainWindow::widgetLayout(QLayout *layout)
{
    QWidget *widget = new QWidget;
    widget->setLayout(layout);
    return widget;
}

void MainWindow::createItemButton(QGridLayout *gLayout, FlowItem::Type type,
                                  QString statusTip)
{
    FlowItem *item;
    if (type <= FlowItem::flowNodes) {
        item = new FlowPolygon(type, QBrush(Qt::white));
    }
    else if (type <= FlowItem::flowLines) {
        item = new FlowLine(type == FlowItem::Type::Arrow);
    }
    else if (type == FlowItem::TextField) {
        item = new TextField();
    }
    else {
        assert(false);
        return;
    }

    QToolButton *button = new QToolButton;

    button->setCheckable(true);
    button->setIcon(QIcon(item->image()));
    button->setStatusTip(statusTip);
    button->setIconSize(QSize(ICON_X, ICON_Y));
    itemButtons->addButton(button, type);
    int count = gLayout->count();
    gLayout->addWidget(button, count, 0, Qt::AlignCenter);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        canvas->clearSelection();
        for (auto & but : itemButtons->buttons()) {
            but->setChecked(false);
            canvas->setMode();
        }
        clearFocus();
    }
    QMainWindow::keyPressEvent(event);
}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// slot methods implementation
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

void MainWindow::itemButtonClicked(int id)
{
    // unselect all items
    canvas->clearSelection();
    // set down other buttons
    auto clickedButton = itemButtons->button(id);
    for (auto &button : itemButtons->buttons()) {
        if (clickedButton != button) {
            button->setChecked(false);
        }
    }

    if (!clickedButton->isChecked()) {
        // button is disabled - disble inserting mode
        canvas->setMode(Canvas::MoveItem);
        canvas->setItemType(FlowItem::Type::None);
        return;
    }

    switch (FlowItem::Type(id)) {
        case FlowItem::Type::TextField:
            canvas->setMode(Canvas::InsertText);
            break;
        case FlowItem::Type::Line:
        case FlowItem::Type::Arrow:
            canvas->setMode(Canvas::InsertLine);
            break;
        default:
            canvas->setMode(Canvas::InsertItem);
    }

    canvas->setItemType(FlowItem::Type(id));
}

void MainWindow::textInserted(QGraphicsTextItem *item) {
    qDebug() << "text inserted";
    Q_UNUSED(item);
    canvas->setMode();
    itemButtons->button(FlowItem::Type::TextField)->setChecked(false);
}

void MainWindow::itemSelected(QGraphicsItem *item) {
    qDebug() << "item selected";
    Q_UNUSED(item);
}

void MainWindow::itemInserted(FlowPolygon *item) {
    Q_UNUSED(item);
    // TODO some stuff for undo operation
}

void MainWindow::arrowInserted() {

}

void MainWindow::deleteItem() {
    for (auto &i : canvas->selectedItems()) {
        canvas->removeItem(i);
    }
}

void MainWindow::putFront() {
    qreal zmax = 0;
    for (auto &i : canvas->items()) {
        if (i->type() == FlowPolygon::Type || i->type() == FlowLine::Type) {
            zmax = i->zValue() > zmax ? i->zValue() : zmax;
        }
    }
    for (auto &i : canvas->selectedItems()) {
        i->setZValue(zmax + 0.001);
    }
}

void MainWindow::putBack() {
    qreal zmin = 0;
    for (auto &i : canvas->items()) {
        if (i->type() == FlowPolygon::Type || i->type() == FlowLine::Type) {
            zmin = i->zValue() < zmin ? i->zValue() : zmin;
        }
    }
    for (auto &i : canvas->selectedItems()) {
        i->setZValue(zmin - 0.001);
    }
}

void MainWindow::copy() {
    for (auto & i : canvas->selectedItems()) {
        if (itemCopy) {
            delete itemCopy;
        }
        if (i->type() == FlowPolygon::Type) {
            itemCopy = new FlowPolygon(static_cast<FlowPolygon*>(i));
        }
        else if (i->type() == FlowLine::Type) {
            itemCopy = new FlowLine(static_cast<FlowLine*>(i));
        }
        else if (i->type() == TextField::Type) {
            itemCopy = new TextField(static_cast<TextField*>(i));
        }
        pasteAction->setChecked(true);
        break;
    }
}

void MainWindow::paste() {
    assert(itemCopy);
    canvas->pasteItem(itemCopy);
}
