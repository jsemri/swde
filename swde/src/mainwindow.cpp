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
    //connect(canvas, SIGNAL(item_inserted()))
    view = new QGraphicsView(canvas);
    view->setDragMode(QGraphicsView::NoDrag);
    view->setCacheMode(QGraphicsView::CacheBackground);
    view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    view->fitInView(0, 0, 400, 400, Qt::KeepAspectRatio);
    view->setEnabled(true);

    layout = new QHBoxLayout();
    layout->addWidget(toolbox);
    layout->addWidget(view);
    setCentralWidget(widget_layout(layout));
    create_toolbars();

    setWindowIcon(QIcon(":/images/swde.png"));
    setWindowTitle("Simple Workflow Diagram Editor");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::create_actions()
{
    // actions in main window toolbars
    exit_action = new QAction(tr("E&xit"), this);
    connect(exit_action, SIGNAL(triggered()), this, SLOT(close()));

    about_action = new QAction(tr("&About"), this);
    connect(about_action, SIGNAL(triggered()), this, SLOT(about()));

    // actions on edit toolbar
    delete_action = new QAction(QIcon(":/images/delete.png"), tr("Delete"),
                                this);
    delete_action->setShortcut(tr("Delete"));
    delete_action->setStatusTip(tr("Delete item from canvas."));
    connect(delete_action, SIGNAL(triggered()), this, SLOT(delete_item()));

    copy_format_action = new QAction(QIcon(":/images/copyformat.png"),
                                     tr("Copy format"), this);
    copy_format_action->setShortcut(tr("Ctrl+f"));
    copy_format_action->setStatusTip(tr("Copy item format to clipboard."));

    copy_action = new QAction(QIcon(":/images/copy.png"), tr("Copy"),
                              this);
    copy_action->setShortcut(tr("Ctrl+c"));
    copy_action->setStatusTip(tr("Copy item to clipboard."));

    paste_action = new QAction(QIcon(":/images/paste.png"), tr("Paste"), this);
    paste_action->setShortcut(tr("Ctrl+p"));
    paste_action->setStatusTip(tr("Paste item from clipboard to canvas."));

    to_front_action = new QAction(QIcon(":/images/tofront.png"),
                                  tr("To Front"), this);
    to_front_action->setStatusTip(tr("Move item to front."));

    to_back_action = new QAction(QIcon(":/images/toback.png"), tr("To Back"),
                                 this);
    to_back_action->setStatusTip(tr("Move item to back."));

    undo_action = new QAction(QIcon(":/images/undo.png"), tr("Undo"),
                              this);
    undo_action->setShortcut(tr("Ctrl+z"));
    undo_action->setStatusTip(tr("Undo current action."));
    undo_action->setChecked(false);
    connect(undo_action, SIGNAL(triggered()), this, SLOT(undo()));
}

void MainWindow::create_toolbars()
{
    // toolbars like in menu
    edit_toolbar = addToolBar(tr("Edit"));
    edit_toolbar->addAction(delete_action);
    edit_toolbar->addAction(copy_format_action);
    edit_toolbar->addAction(copy_action);
    edit_toolbar->addAction(paste_action);
    edit_toolbar->addAction(to_front_action);
    edit_toolbar->addAction(to_back_action);
    edit_toolbar->addAction(undo_action);

    scale_combo = new QComboBox;
    QStringList scales;
    scales << tr("50%") << tr("75%") << tr("100%") << tr("125%") << tr("150%");
    scale_combo->addItems(scales);
    scale_combo->setCurrentIndex(2);
    connect(scale_combo, SIGNAL(currentIndexChanged(QString)), this,
                                SLOT(scale_changed(QString)));

    edit_toolbar->addWidget(scale_combo);

    // creating a color button with popup menu
    // color of item is changed either when button clicked or color is picked
    // from popup menu
    fill_color_button = new QToolButton;
    fill_color_button->setPopupMode(QToolButton::MenuButtonPopup);
    fill_color_button->setMenu(create_color_menu(SLOT(item_color_changed())));
    fill_action = fill_color_button->menu()->defaultAction();
    fill_color_button->setIcon(create_color_icon());
    connect(fill_color_button, SIGNAL(clicked()), this,
            SLOT(fill_button_triggered()));

    item_toolbar = addToolBar(tr("Color"));
    item_toolbar->addWidget(fill_color_button);



    // @@@@@@@@@@@@@@@@@@@@@@
    // font and text buttons
    // @@@@@@@@@@@@@@@@@@@@@@
    /*
    font_combo = new QFontComboBox;
    // XXX connect to change font
    font_size_combo = new QComboBox;
    font_size_combo->setEditable(true);
    for (int i = 8; i < 32; i += 2) {
        font_size_combo->addItem(QString().setNum(i));
    }
    QIntValidator *validator = new QIntValidator(2, 64, this);
    font_size_combo->setValidator(validator);
    // XXX connect to change font size
    font_color_button = new QToolButton;
    font_color_button->setPopupMode(QToolButton::MenuButtonPopup);
    font_color_button->setMenu(create_color_menu(SLOT(item_color_changed())));
    //fill_color_button->setIcon(te));
    font_toolbar = addToolBar(tr("Font"));
    font_toolbar->addWidget(font_color_button);
    */
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
    item_buttons->addButton(text_button, FlowChartItem::None);
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

void MainWindow::create_menus()
{
    file_menu = menuBar()->addMenu(tr("&File"));
    file_menu->addAction(exit_action);
    about_menu = menuBar()->addMenu(tr("&Help"));
    about_menu->addAction(about_action);
}

QMenu *MainWindow::create_color_menu(const char *slot, QColor default_color)
{
    // colors and names
    QList<QColor> colors;
    colors << Qt::black << Qt::white << Qt::red << Qt::blue << Qt::green;
    QStringList names;
    names << tr("black") << tr("white") << tr("red") << tr("blue")
          << tr("green");

    // this must hold
    assert(names.count() == colors.count());

    // creating a menu
    QMenu *color_menu = new QMenu(this);
    for (int i = 0; i < colors.count(); i++) {
        QColor col = colors.at(i);
        // action which changes color of a button
        QAction *action = new QAction(names.at(i), this);
        action->setData(col);
        action->setIcon(create_color_icon(col, 20));
        connect(action, SIGNAL(triggered()), this, slot);
        color_menu->addAction(action);
        if (col == default_color) {
            color_menu->setDefaultAction(action);
        }
    }

    return color_menu;
}

QIcon MainWindow::create_color_icon(QColor color, int size)
{
    QPixmap pixmap(size, size);
    pixmap.fill(color);
    /*QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.fillRect(QRect(0, 0, 20, 20), col);*/
    return QIcon(pixmap);
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
    FlowChartItem item(type, Qt::white, edit_menu);
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
        clear_focus();
    }
    QMainWindow::keyPressEvent(event);
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

void MainWindow::item_color_changed()
{
    fill_action = qobject_cast<QAction *>(sender());
    fill_color_button->setIcon(create_color_icon(qvariant_cast<QColor>(
                                                     fill_action->data())));
    fill_button_triggered();
}

void MainWindow::fill_button_triggered()
{
    for (auto & item : canvas->selectedItems()) {
        static_cast<FlowChartItem*>(item)->change_color(
                    qvariant_cast<QColor>(fill_action->data()));
    }
    canvas->set_color(qvariant_cast<QColor>(fill_action->data()));
}

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
    else {
        // button is enabled now - enable item insertion
        canvas->set_mode(Canvas::InsertItem);
        canvas->set_item_type(FlowChartItem::FlowChartItemType(id));
    }
}

// changes old scale to new one by resizing all items on canvas
void MainWindow::scale_changed(const QString &scale) {
    double new_scale = scale.left(scale.indexOf(tr("%"))).toDouble() / 100.0;
    QMatrix old_matrix = view->matrix();
    view->resetMatrix();
    view->translate(old_matrix.dx(), old_matrix.dy());
    view->scale(new_scale, new_scale);
}

void MainWindow::clear_focus() {
    for (auto & but : item_buttons->buttons()) {
        but->setChecked(false);
        canvas->set_mode();
    }
}
