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

#include <algorithm>
#include <cassert>

#include "mainwindow.h"
#include "canvas.h"
#include "flowchartitem.h"
#include "debug.h"

#include "ui_mainwindow.h"

#define ICON_SIZE 35

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    create_actions();
    create_menus();
    create_toolbox();

    canvas = new Canvas(edit_menu, this);
    connect(canvas, SIGNAL(textInserted(QGraphicsTextItem*)), this,
            SLOT(textInserted(QGraphicsTextItem*)));

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
    setCentralWidget(widget_layout(layout));
    setWindowIcon(QIcon(":/images/swde.png"));
    setWindowTitle("Simple Workflow Diagram Editor");
    setMinimumSize(600, 400);
    //setGeometry(qApp->desktop()->availableGeometry());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::create_actions()
{
    exit_action = new QAction(tr("E&xit"), this);
    connect(exit_action, SIGNAL(triggered()), this, SLOT(close()));
}

// create buttons on the right
// buttons manage which diagram element will be inserted
void MainWindow::create_toolbox()
{
    // group of buttons which manage diagram elements
    item_buttons = new QButtonGroup(this);
    item_buttons->setExclusive(false);
    connect(item_buttons, SIGNAL(buttonClicked(int)), this,
            SLOT(item_button_clicked(int)));

    QGridLayout *gl = new QGridLayout;
    gl->addWidget(create_cell_widget(tr("Process"), FlowChartItem::Process),
                  0, 0);
    gl->addWidget(create_cell_widget(tr("Conditional"), FlowChartItem::Condition),
                  0, 1);

    QToolButton *text_button = new QToolButton;
    text_button->setCheckable(true);
    text_button->setIconSize(QSize(ICON_SIZE, ICON_SIZE));
    item_buttons->addButton(text_button, FlowChartItem::Text);
    QGridLayout *text_layout = new QGridLayout;
    text_layout->addWidget(text_button, 0, 0, Qt::AlignCenter);
    text_layout->addWidget(new QLabel(tr("Text")),1 ,0 , Qt::AlignCenter);

    gl->addWidget(widget_layout(text_layout), 2, 0);
    gl->setRowStretch(3, 10);
    gl->setColumnStretch(2, 10);
    QWidget *widget = widget_layout(gl);

    toolbox = new QToolBox();
    toolbox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,
                                       QSizePolicy::Ignored));
    toolbox->setMinimumWidth(widget->sizeHint().width());
    toolbox->addItem(widget, tr("afsd"));
    QColor col(3, 146, 13);
    toolbox->setPalette(QPalette(col));
    toolbox->setFrameShape(QFrame::WinPanel);
}

void MainWindow::create_menus()
{
    file_menu = menuBar()->addMenu(tr("&File"));
    file_menu->addAction(exit_action);
}

QWidget *MainWindow::widget_layout(QLayout *layout)
{
    QWidget *widget = new QWidget;
    widget->setLayout(layout);
    return widget;
}

// create button with item insertion
QWidget *
MainWindow::create_cell_widget(const QString &text,
                               FlowChartItem::FlowChartItemType type)
{
    FlowChartItem item(type, Qt::white, 1, edit_menu);
    QIcon icon(item.image());

    //QToolButton *button = new QToolButton;
    QPushButton *button = new QPushButton;
    button->setIcon(icon);
    button->setIconSize(QSize(ICON_SIZE, ICON_SIZE));
    button->setCheckable(true);
    item_buttons->addButton(button, int(type));

    QGridLayout *layout = new QGridLayout;
    //layout->addWidget(button);
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

    return widget_layout(layout);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        canvas->clearSelection();
        for (auto & but : item_buttons->buttons()) {
            but->setChecked(false);
            canvas->set_mode();
        }
        clearFocus();
    }
    QMainWindow::keyPressEvent(event);
}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// slot methods implementation
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

void MainWindow::item_button_clicked(int id)
{
    // set down other buttons
    auto clicked_button = item_buttons->button(id);
    for (auto &button : item_buttons->buttons()) {
        if (clicked_button != button) {
            button->setChecked(false);
        }
    }

    if (!clicked_button->isChecked()) {
        // button is disabled - disble inserting mode
        canvas->set_mode(Canvas::MoveItem);
    }
    else if (FlowChartItem::FlowChartItemType(id) == FlowChartItem::None) {
        // enable text insertion
        canvas->set_mode(Canvas::MoveItem);
    }
    else if (FlowChartItem::FlowChartItemType(id) == FlowChartItem::Text) {
        canvas->set_mode(Canvas::InsertText);
    }
    else {
        // button is enabled now - enable item insertion
        canvas->set_mode(Canvas::InsertItem);
        canvas->set_item_type(FlowChartItem::FlowChartItemType(id));
    }
}

void MainWindow::textInserted(QGraphicsTextItem *item) {
    qDebug() << "text inserted";
}

void MainWindow::itemSelected(QGraphicsItem *item) {
    qDebug() << "item selected";
}
