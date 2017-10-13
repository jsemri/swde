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
    QMainWindow(parent)
    //ui(new Ui::MainWindow)
{
    //ui->setupUi(this);
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
    view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    view->setEnabled(true);

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

}

void MainWindow::createActions()
{
    exitAction = new QAction(tr("E&xit"), this);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
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
        item = new FlowPolygon(type, Qt::white, 1, editMenu);
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
