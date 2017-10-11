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

#include "mainwindow.h"
#include "canvas.h"
#include "flowchartitem.h"

#define ICON_SIZE 35

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
    connect(canvas, SIGNAL(itemInserted(FlowChartItem*)), this,
            SLOT(itemInserted(FlowChartItem*)));
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

    QGridLayout *gl = new QGridLayout;
    gl->addWidget(createCellWidget(tr("Process"), FlowChartItem::Process),
                  0, 0);
    gl->addWidget(createCellWidget(tr("Conditional"), FlowChartItem::Condition),
                  0, 1);

    QToolButton *textButton = new QToolButton;
    textButton->setCheckable(true);
    textButton->setIconSize(QSize(ICON_SIZE, ICON_SIZE));
    itemButtons->addButton(textButton, FlowChartItem::Text);
    QGridLayout *textLayout = new QGridLayout;
    textLayout->addWidget(textButton, 0, 0, Qt::AlignCenter);
    textLayout->addWidget(new QLabel(tr("Text")),1 ,0 , Qt::AlignCenter);

    gl->addWidget(widgetLayout(textLayout), 2, 0);
    gl->setRowStretch(3, 10);
    gl->setColumnStretch(2, 10);
    QWidget *widget = widgetLayout(gl);

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

// create button with item insertion
QWidget *
MainWindow::createCellWidget(
        const QString &text,
        FlowChartItem::FlowChartItemType type)
{
    FlowChartItem item(type, Qt::white, 1, editMenu);
    QIcon icon(item.image());

    //QToolButton *button = new QToolButton;
    QPushButton *button = new QPushButton;
    button->setIcon(icon);
    button->setIconSize(QSize(ICON_SIZE, ICON_SIZE));
    button->setCheckable(true);
    itemButtons->addButton(button, int(type));

    QGridLayout *layout = new QGridLayout;
    //layout->addWidget(button);
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

    return widgetLayout(layout);
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
    }
    else if (FlowChartItem::FlowChartItemType(id) == FlowChartItem::None) {
        // enable text insertion
        canvas->setMode(Canvas::MoveItem);
    }
    else if (FlowChartItem::FlowChartItemType(id) == FlowChartItem::Text) {
        canvas->setMode(Canvas::InsertText);
    }
    else if (FlowChartItem::FlowChartItemType(id) == FlowChartItem::Line) {
        canvas->setMode(Canvas::InsertLine);
    }
    else {
        // button is enabled now - enable item insertion
        canvas->setMode(Canvas::InsertItem);
        canvas->setItemType(FlowChartItem::FlowChartItemType(id));
    }
}

void MainWindow::textInserted(QGraphicsTextItem *item) {
    qDebug() << "text inserted";
    canvas->setMode();
    itemButtons->button(FlowChartItem::Text)->setChecked(false);
}

void MainWindow::itemSelected(QGraphicsItem *item) {
    qDebug() << "item selected";
}

void MainWindow::itemInserted(FlowChartItem *item) {
  /*  canvas->setMode();
    itemButtons->button(int(item->getType()))->setChecked(false);*/
}

void MainWindow::arrowInserted() {

}
