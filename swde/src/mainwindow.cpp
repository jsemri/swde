#include <QGraphicsView>
#include <QHBoxLayout>
#include <QToolBox>
#include <QToolButton>
#include <QLabel>
#include <QDebug>
#include <QPushButton>
#include <QComboBox>
#include <QToolBar>
#include <QKeyEvent>
#include <QDesktopWidget>
#include <QApplication>
#include <QAction>
#include <QButtonGroup>
#include <QMenuBar>
#include <QMenu>
#include <QDialog>
#include <QMessageBox>
#include <QFileDialog>

#include <algorithm>
#include <cassert>

#include "flowitem.h"
#include "mainwindow.h"
#include "canvas.h"
#include "flowpolygon.h"
#include "flowline.h"
#include "textfield.h"
#include "newfiledialog.h"

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
    view->fitInView(0, 0, 500, 400, Qt::KeepAspectRatio);
    view->setDragMode(QGraphicsView::NoDrag);
    view->setCacheMode(QGraphicsView::CacheBackground);
    //view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    view->setEnabled(true);
    view->setFrameRect(QRect(0,0,1000,1000));
    view->setFrameStyle(QFrame::Box);
    view->setFrameShadow(QFrame::Raised);

    createDialogs();
    createToolbars();
    layout = new QHBoxLayout();
    layout->addWidget(toolbox);
    layout->addWidget(view);
    setCentralWidget(widgetLayout(layout));
    setWindowIcon(QIcon(":/images/swde.png"));
    setWindowTitle("Simple Workflow Diagram Editor");
    setMinimumSize(600, 400);
    setGeometry(qApp->desktop()->availableGeometry());
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
    pasteAction->setEnabled(false);

    toBoldAction = new QAction(QIcon(":/images/bold.png"), tr("Bold"), this);
    toBoldAction->setCheckable(true);
    connect(toBoldAction, SIGNAL(triggered()), this, SLOT(changeFont()));
    toItalicAction = new QAction(QIcon(":/images/italic.png"), tr("Italic"),
                                 this);
    toItalicAction->setCheckable(true);
    connect(toItalicAction, SIGNAL(triggered()), this, SLOT(changeFont()));
    toUnderlineAction = new QAction(QIcon(":/images/underline.png"),
                                    tr("Underline"), this);
    toUnderlineAction->setCheckable(true);
    connect(toUnderlineAction, SIGNAL(triggered()), this, SLOT(changeFont()));

    newAction = new QAction(QIcon(":/images/new.png"), tr("new"), this);
    connect(newAction, SIGNAL(triggered(bool)), this, SLOT(newFile()));
    loadAction = new QAction(QIcon(":/images/load.png"), tr("load"), this);
    connect(loadAction, SIGNAL(triggered(bool)), this, SLOT(loadFile()));
    saveAction = new QAction(QIcon(":/images/save.png"), tr("save"), this);
    connect(saveAction, SIGNAL(triggered(bool)), this, SLOT(saveFile()));
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
    toolbox->addItem(widget, tr("Components"));
    QColor col(3, 146, 13);
    toolbox->setPalette(QPalette(col));
    toolbox->setFrameShape(QFrame::WinPanel);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(exitAction);
    fileMenu->addAction(newAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(loadAction);
}

void MainWindow::createToolbars() {

    // save, load, new
    fileToolbar = addToolBar("file");
    fileToolbar->addAction(newAction);
    fileToolbar->addAction(loadAction);
    fileToolbar->addAction(saveAction);

    editToolbar = addToolBar(tr("Edit Item"));
    editToolbar->addAction(deleteAction);
    editToolbar->addAction(copyAction);
    editToolbar->addAction(pasteAction);

    // aspects
    aspectToolbar = addToolBar(tr("Aspect"));
    aspectToolbar->addAction(putFrontAction);
    aspectToolbar->addAction(putBackAction);

    QComboBox *scaleCombo = new QComboBox;
    QStringList scales;
    scales << tr("25%") << tr("50%") << tr("75%") << tr("100%") << tr("125%")
           << tr("150%") << tr("200%") << tr("300%") << tr("400%");
    scaleCombo->addItems(scales);
    scaleCombo->setCurrentIndex(3);
    connect(scaleCombo, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(scaleChanged(QString)));
    aspectToolbar->addWidget(scaleCombo);

    // nodes and arrows color & border toolbar
    // shape color
    colorButton = new QToolButton;
    colorButton->setPopupMode(QToolButton::MenuButtonPopup);
    colorButton->setMenu(createColorMenu(SLOT(changeColor()), Qt::white));
    changeColorAction = colorButton->menu()->defaultAction();
    colorButton->setIcon(createColorIcon(Qt::white));
    connect(colorButton, SIGNAL(clicked()), this,
            SLOT(colorButtonTriggered()));

    // border color
    borderColorButton = new QToolButton;
    borderColorButton->setPopupMode(QToolButton::MenuButtonPopup);
    borderColorButton->setMenu(createColorMenu(SLOT(changeBorderColor()), Qt::black));
    changeBorderColorAction = borderColorButton->menu()->defaultAction();
    borderColorButton->setIcon(createBorderIcon(Qt::black, 2));
    connect(borderColorButton, SIGNAL(clicked()), canvas,
            SLOT(borderButtonClicked()));

    // border width
    borderWidthCombo = new QComboBox;
    for (int i = 1; i < 9; i++) {
        borderWidthCombo->addItem(createBorderIcon(Qt::black, i), QString().setNum(i), i);
    }
    borderWidthCombo->setCurrentIndex(1);
    connect(borderWidthCombo, SIGNAL(currentIndexChanged(int)),
            canvas, SLOT(penWidthChanged(int)));

    itemToolbar = addToolBar(tr("Color and Borders"));
    itemToolbar->addWidget(colorButton);
    itemToolbar->addWidget(borderColorButton);
    itemToolbar->addWidget(borderWidthCombo);

    // text
    fontSizeCombo = new QComboBox;
    for (int i = 8; i < 30; i+=2) {
        fontSizeCombo->addItem(QString().setNum(i));
    }
    fontSizeCombo->setCurrentIndex(2);
    connect(fontSizeCombo, SIGNAL(currentIndexChanged(QString)), this,
            SLOT(fontSizeChanged(QString)));

    textToolbar = addToolBar(tr("Text Editing"));
    textToolbar->addWidget(fontSizeCombo);
    textToolbar->addAction(toBoldAction);
    textToolbar->addAction(toItalicAction);
    textToolbar->addAction(toUnderlineAction);
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
        item = new FlowPolygon(type);
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

QMenu *MainWindow::createColorMenu(const char *slot, QColor defaultColor)
{
    QList<QColor> colors;
    colors << Qt::black << Qt::white << Qt::lightGray << Qt::cyan << Qt::blue
           << Qt::darkMagenta << Qt::red << QColor(255,128,0) << Qt::darkYellow;

    QMenu *colorMenu = new QMenu(this);
    for (auto & i : colors) {
        QAction *action = new QAction(this);
        action->setData(i);
        action->setIcon(createColorIcon(i));
        connect(action, SIGNAL(triggered()), this, slot);
        colorMenu->addAction(action);
        if (i == defaultColor) {
            colorMenu->setDefaultAction(action);
        }
    }
    colorMenu->setMaximumWidth(30);

    return colorMenu;
}

QIcon MainWindow::createColorIcon(QColor color)
{
    QPixmap pixmap(20, 20);
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.fillRect(QRect(0, 0, 20, 20), QBrush(color));

    return QIcon(pixmap);
}

QIcon MainWindow::createBorderIcon(QColor color, int width)
{
    QPixmap pixmap(20, 20);
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.fillRect(QRect(0,0,20,20), Qt::white);
    painter.setPen(QPen(color, width));
    painter.drawLine(0, 0, 20, 20);

    return QIcon(pixmap);
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
        pasteAction->setEnabled(true);
        break;
    }
}

void MainWindow::paste() {
    assert(itemCopy);
    canvas->pasteItem(itemCopy);
}

void MainWindow::scaleChanged(const QString &scale)
{
    QMatrix m = view->matrix();
    double s = scale.left(scale.indexOf(tr("%"))).toDouble() / 100.0;
    view->resetMatrix();
    view->translate(m.dx(), m.dy());
    view->scale(s, s);
}

void MainWindow::changeBorderColor() {
    changeBorderColorAction = qobject_cast<QAction *>(sender());
    QColor color = qvariant_cast<QColor>(changeBorderColorAction->data());
    borderColorButton->setIcon(createBorderIcon(color, 2));
    canvas->penColorChanged(color);
}

void MainWindow::changeColor() {
    changeColorAction = qobject_cast<QAction *>(sender());
    QColor color = qvariant_cast<QColor>(changeColorAction->data());
    colorButton->setIcon(createColorIcon(color));
    canvas->setItemColor(color);
    colorButtonTriggered();
}

void MainWindow::colorButtonTriggered() {
    for (auto &i : canvas->selectedItems()) {
        QColor color = qvariant_cast<QColor>(changeColorAction->data());
        if (i->type() == FlowPolygon::Type) {
            static_cast<FlowPolygon*>(i)->changeColor(color);
        }
    }
}

void MainWindow::changeFont() {
    qDebug() << "changing font";
    QFont font = canvas->font();
    font.setPointSize(fontSizeCombo->currentText().toInt());
    font.setItalic(toItalicAction->isChecked());
    font.setWeight(toBoldAction->isChecked() ? QFont::Bold : QFont::Normal);
    font.setUnderline(toUnderlineAction->isChecked());

    canvas->setFont(font);
}

void MainWindow::fontSizeChanged(QString s) {
    Q_UNUSED(s);
    changeFont();
}

void MainWindow::createDialogs() {
    newDialog = new NewFileDialog(this);
    msgBox = new QMessageBox(this);
    msgBox->setWindowTitle("Warning");
    msgBox->setText("The file has been modified.");
    msgBox->setInformativeText("Do you want to save chages?");
    msgBox->setStandardButtons(QMessageBox::Cancel | QMessageBox::Save |
                                  QMessageBox::Discard);
    msgBox->setDefaultButton(QMessageBox::Cancel);
}

void MainWindow::newFile() {
    if (newDialog->exec() == QDialog::Accepted) {
        if (canvas->items().count() > 4) {
            // show warning dialog
            int ret = msgBox->exec();
            if (ret == QMessageBox::Cancel) {
                return;
            }
            else if (ret == QMessageBox::Save) {
                saveFile();
            }
        }
        canvas->resize(newDialog->getWidth(), newDialog->getHeight());
    }
}

void MainWindow::loadFile() {
    QString loadFile = QFileDialog::getOpenFileName(this, tr("Load File"), "..");
    try {
        canvas->load(loadFile);
    }
    catch (std::exception &e) {
        QMessageBox msg;
        msg.critical(0, "Error", QString(e.what()));
    }
}

void MainWindow::saveFile() {
    QString saveFile = QFileDialog::getSaveFileName(this, tr("Save File"), "..");
    if (saveFile == "") {
        return;
    }

    try {
        canvas->save(saveFile);
    }
    catch (std::exception &e) {
        QMessageBox msg;
        msg.critical(0, "Error", QString(e.what()));
    }
}
