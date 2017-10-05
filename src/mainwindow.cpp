#include <QGraphicsView>
#include <QHBoxLayout>
#include <QToolBox>
#include <QToolButton>
#include <QLabel>
#include <QDebug>

#include "mainwindow.h"
#include "canvas.h"
#include "component.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    create_menus();
    create_toolbox();

    canvas = new Canvas(this);
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
    QWidget *central = new QWidget;
    central->setLayout(layout);
    setCentralWidget(central);

    setWindowTitle("Simple Workflow Diagram Editor");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::create_actions() {

}

void MainWindow::create_toolbox() {

    button_gr = new QButtonGroup(this);
    button_gr->setExclusive(false);
    // connect, action when clicked

    QGridLayout *gl = new QGridLayout;
    gl->addWidget(create_cell_widget(tr("Conditional"), Component::Condition), 0, 0);
    gl->addWidget(create_cell_widget(tr("Conditional"), Component::Condition), 0, 1);
    gl->addWidget(create_cell_widget(tr("Conditional"), Component::Condition), 1, 1);
    gl->addWidget(create_cell_widget(tr("Conditional"), Component::Condition), 1, 0);

    QToolButton *text_button = new QToolButton;
    text_button->setCheckable(true);
    text_button->setIconSize(QSize(50, 50));
    button_gr->addButton(text_button);
    // XXX add more
    gl->setRowStretch(3, 10);
    gl->setColumnStretch(2, 10);
    QWidget *widget = new QWidget;
    widget->setLayout(gl);

    toolbox = new QToolBox();
    toolbox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,
                                       QSizePolicy::Ignored));
    toolbox->setMinimumWidth(widget->sizeHint().width());
    toolbox->addItem(widget, tr("afsd"));
}

void MainWindow::create_menus() {
    QAction *exit_action = new QAction(tr("E&xit"), this);
    connect(exit_action, SIGNAL(triggered()), this, SLOT(close()));
    file_menu = menuBar()->addMenu(tr("&File"));
    file_menu->addAction(exit_action);

    QAction *undo_action = new QAction(tr("&Undo"), this);
    connect(undo_action, SIGNAL(triggered()), this, SLOT(undo()));
    edit_menu = menuBar()->addMenu(tr("&Edit"));
    edit_menu->addAction(undo_action);

    QAction *about_action = new QAction(tr("&About"), this);
    connect(about_action, SIGNAL(triggered()), this, SLOT(about()));
    about_menu = menuBar()->addMenu(tr("&Help"));
    about_menu->addAction(about_action);
}

QWidget *MainWindow::create_bg_cell_widget
(const QString &text, const QString &image)
{
    QToolButton *button = new QToolButton;
    button->setText(text);
    button->setIcon(QIcon(image));
    button->setIconSize(QSize(50, 50));
    button->setCheckable(true);
    bg_button_gr->addButton(button);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    return widget;
}

QWidget *MainWindow::create_cell_widget
(const QString &text, Component::ComponentType type)
{
    Component item(type, edit_menu);
    QIcon icon(item.image());

    QToolButton *button = new QToolButton;
    button->setIcon(icon);
    button->setIconSize(QSize(50, 50));
    button->setCheckable(true);
    button_gr->addButton(button, int(type));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    return widget;
}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// slot methods implementation
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

void MainWindow::undo() {

}

void MainWindow::about() {

}
