#include <QGraphicsView>
#include <QHBoxLayout>
#include <QToolBox>
#include <QToolButton>
#include <QLabel>
#include <QDebug>

#include "mainwindow.h"
#include "canvas.h"
#include "diagramnode.h"

#include "ui_mainwindow.h"

#define ICON_SIZE 35
#define TEXT_BUTTON_ID 120

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    create_actions();
    create_menus();
    create_toolbox();

    canvas = new Canvas(edit_menu, this);
    view = new QGraphicsView(canvas);
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

    setWindowTitle("Simple Workflow Diagram Editor");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::create_actions() {
    exit_action = new QAction(tr("E&xit"), this);
    connect(exit_action, SIGNAL(triggered()), this, SLOT(close()));

    undo_action = new QAction(tr("&Undo"), this);
    connect(undo_action, SIGNAL(triggered()), this, SLOT(undo()));

    about_action = new QAction(tr("&About"), this);
    connect(about_action, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::create_toolbox() {

    node_buttons = new QButtonGroup(this);
    node_buttons->setExclusive(true);
    // connect, action when clicked
    connect(node_buttons, SIGNAL(buttonClicked(int)), this,
            SLOT(node_button_clicked(int)));

    QGridLayout *gl = new QGridLayout;
    gl->addWidget(create_cell_widget(tr("Conditional"), DiagramNode::Condition),
                  0, 0);
    gl->addWidget(create_cell_widget(tr("Conditional"), DiagramNode::Condition),
                  0, 1);
    gl->addWidget(create_cell_widget(tr("Conditional"), DiagramNode::Condition),
                  1, 1);
    gl->addWidget(create_cell_widget(tr("Conditional"), DiagramNode::Condition),
                  1, 0);

    QToolButton *text_button = new QToolButton;
    text_button->setCheckable(true);
    text_button->setIconSize(QSize(ICON_SIZE, ICON_SIZE));
    node_buttons->addButton(text_button, TEXT_BUTTON_ID);
    QGridLayout *text_layout = new QGridLayout;
    text_layout->addWidget(text_button, 0, 0, Qt::AlignCenter);
    text_layout->addWidget(new QLabel(tr("Text")),1 ,0 , Qt::AlignCenter);

    gl->addWidget(widget_layout(text_layout), 2, 0);
//Qt::Align
    // XXX add more
    gl->setRowStretch(3, 10);
    gl->setColumnStretch(2, 10);
    QWidget *widget = widget_layout(gl);

    toolbox = new QToolBox();
    toolbox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,
                                       QSizePolicy::Ignored));
    toolbox->setMinimumWidth(widget->sizeHint().width());
    toolbox->addItem(widget, tr("afsd"));
    QColor col(168, 136, 7);
    toolbox->setPalette(QPalette(col));
}

void MainWindow::create_menus() {

    file_menu = menuBar()->addMenu(tr("&File"));
    file_menu->addAction(exit_action);

    edit_menu = menuBar()->addMenu(tr("&Edit"));
    edit_menu->addAction(undo_action);

    about_menu = menuBar()->addMenu(tr("&Help"));
    about_menu->addAction(about_action);
}

QWidget *MainWindow::widget_layout(QLayout *layout) {
    QWidget *widget = new QWidget;
    widget->setLayout(layout);
    return widget;
}

QWidget *MainWindow::create_bg_cell_widget
(const QString &text, const QString &image)
{
    QToolButton *button = new QToolButton;
    button->setText(text);
    button->setIcon(QIcon(image));
    button->setIconSize(QSize(ICON_SIZE, ICON_SIZE));
    button->setCheckable(true);
    background_buttons->addButton(button);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    return widget;
}

QWidget *
MainWindow::create_cell_widget(const QString &text,
                               DiagramNode::DiagramNodeType type)
{
    DiagramNode item(type, edit_menu);
    QIcon icon(item.image());

    QToolButton *button = new QToolButton;
    button->setIcon(icon);
    button->setIconSize(QSize(ICON_SIZE, ICON_SIZE));
    button->setCheckable(true);
    node_buttons->addButton(button, int(type));

    QGridLayout *layout = new QGridLayout;
    //layout->addWidget(button);
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

    return widget_layout(layout);
}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// slot methods implementation
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

void MainWindow::undo() {

}

void MainWindow::about() {

}

void MainWindow::delete_item() {

}

void MainWindow::node_button_clicked(int id) {
    if (DiagramNode::DiagramNodeType(id) == DiagramNode::None) {

    }
    else {
        canvas->set_mode(Canvas::InsertNode);
        canvas->set_node_type(DiagramNode::DiagramNodeType(id));
    }
}
