#include <QGraphicsView>
#include <QHBoxLayout>

#include "mainwindow.h"
#include "canvas.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    canvas = new Canvas(this);
    view = new QGraphicsView(canvas);
    view->setDragMode(QGraphicsView::NoDrag);
    view->setCacheMode(QGraphicsView::CacheBackground);
    view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    view->setEnabled(true);

    layout = new QHBoxLayout();
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
