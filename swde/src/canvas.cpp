#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QWidget>
#include <QRect>
#include <QDebug>
#include <QTextCursor>
#include <QGraphicsView>

#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <cassert>

#include <math.h>

#include "mainwindow.h"
#include "flowline.h"
#include "canvas.h"
#include "textfield.h"
#include "aux.h"

Canvas::Canvas(QMenu *itemMenu, QWidget *parrent) :
    QGraphicsScene{parrent}, itemMenu{itemMenu},
    modified{0}, mode{MoveItem}, itemType{FlowItem::Type::None},
    activeItem{nullptr}, itemColor(Qt::white), itemPen{QPen(Qt::black, 1)},
    ZValue{0}
{
    textFont.setPointSize(12);
    resize(1000, 1000);
    setBackgroundBrush(Qt::white);
}

void Canvas::mousePressEvent(QGraphicsSceneMouseEvent *event) {

    FlowPolygon *item;
    TextField *text;
    switch (mode) {
        case InsertItem:
            qDebug() << "inserting item";
            item = new FlowPolygon(itemType, itemColor, itemPen);
            addItem(item);
            item->setPos(event->scenePos());
            item->setZValue(getZValue());
            getInside(item);
            addToHistory(new InsertDeleteCommand(item, this, true));
            break;
        case MoveItem:
        {
            // resize action
            for (auto & it : selectedItems()) {
                if (it->type() == FlowPolygon::Type) {
                    qreal x = event->scenePos().x();
                    qreal y = event->scenePos().y();
                    QPointF corner = it->boundingRect().bottomRight();
                    qreal cx = corner.x() + it->x();//activeItem->x();
                    qreal cy = corner.y() + it->y();//activeItem->y();
                    if ( cx > x && cx - x < 20 && cy > y && cy - y < 20)
                    {
                        // rescaling
                        qDebug() << "switched to resize mode";
                        QPolygonF p = static_cast<FlowPolygon*>(it)->polygon();
                        addToHistory(new RatioChangeCommand(it, p));
                        mode = ResizeItem;
                        activeItem = it;
                        modified = true;
                        return;
                    }
                }
            }
            qDebug() << "moving item";
            if ((activeItem = itemAt(event->scenePos(), QTransform()))) {
                if (activeItem && activeItem->type() == FlowLine::Type) {
                    // moving arrow
                    arrow = static_cast<FlowLine*>(activeItem);
                    qreal x = event->scenePos().x();
                    qreal y = event->scenePos().y();
                    if (fabs(arrow->line().p1().x() + arrow->x() - x) < 15 &&
                        fabs(arrow->line().p1().y() + arrow->y() - y) < 15)
                    {
                        addToHistory(new MoveCommand(arrow, arrow->line().p1(), true));
                        mode = MoveLineP1;
                    }
                    else if (fabs(arrow->line().p2().x() + arrow->x() - x) < 15 &&
                             fabs(arrow->line().p2().y() + arrow->y() - y) < 15)
                    {
                        addToHistory(new MoveCommand(arrow, arrow->line().p2()));
                        mode = MoveLineP2;
                    }
                    else {
                        addToHistory(new MoveCommand(activeItem, activeItem->x(),
                                                     activeItem->y()));
                    }
                }
                else {
                    addToHistory(new MoveCommand(activeItem, activeItem->x(),
                                                 activeItem->y()));
                }
            }

            QGraphicsScene::mousePressEvent(event);
            break;
        }
        case InsertLine:
            qDebug() << "inserting line";
            arrow = new FlowLine(itemType == FlowItem::Type::Arrow,
                                 itemPen,
                                 event->scenePos(),event->scenePos());
            addItem(arrow);
            arrow->setZValue(getZValue());
            break;
        case InsertText:
            qDebug() << "inserting text";
            text = new TextField();
            text->setZValue(1001 + getZValue());
            text->setFont(textFont);
            text->setTextInteractionFlags(Qt::TextEditorInteraction);
            addItem(text);
            text->setPos(event->scenePos());
            connect(text, SIGNAL(lostFocus(TextField*)), this,
                    SLOT(editorLostFocus(TextField*)));
            connect(text, SIGNAL(selectedChange(QGraphicsItem*)), this,
                    SIGNAL(itemSelected(QGraphicsItem*)));
            addToHistory(new InsertDeleteCommand(text, this, true));
            emit textInserted(text);
            QGraphicsScene::mousePressEvent(event);
            break;
        default:
            qDebug() << "undefined operation";
            QGraphicsScene::mousePressEvent(event);
            return;
    };
    modified = true;
}

void Canvas::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    switch (mode) {
        case MoveItem:
            if (activeItem) {
                QGraphicsScene::mouseMoveEvent(event);
                getInside(activeItem);
            }
            break;
        case ResizeItem:
            {
                assert(activeItem);
                QPointF corner = activeItem->boundingRect().bottomLeft();
                qreal xratio = (event->scenePos().x() - activeItem->x())
                               / corner.x();
                qreal yratio = (event->scenePos().y() - activeItem->y())
                               / corner.y();
                static_cast<FlowPolygon*>(activeItem)->changeSize(xratio, yratio);
                getInside(activeItem);
                break;
            }
        case InsertLine:
            // arrow has to be set before it is modified
            if (!arrow) {
                break;
            }
        case MoveLineP2:
            arrow->setEndPoint(getInside(event->scenePos()));
            arrow->update();
            break;
        case MoveLineP1:
            arrow->setBeginPoint(getInside(event->scenePos()));
            arrow->update();
        default:
            return;
    }
}

void Canvas::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {

    switch (mode) {
        case ResizeItem:
            mode = MoveItem;
            break;
        case MoveLineP2:
        case MoveLineP1:
            mode = MoveItem;
        case InsertLine:
            addToHistory(new InsertDeleteCommand(arrow, this, true));
            arrow = 0;
        case MoveItem:
            break;
        default:
            return;
    }
    QGraphicsScene::mouseReleaseEvent(event);
}

void Canvas::editorLostFocus(TextField *item) {
    qDebug() << "lost focus";
    QTextCursor cursor = item->textCursor();
    cursor.clearSelection();
    item->setTextCursor(cursor);
    if (item->toPlainText().isEmpty()) {
        removeItem(item);
        item->deleteLater();
    }
}

QPointF Canvas::getInside(QPointF point) const {
    qreal x = point.x();
    qreal y = point.y();
    if (x < 0) {
        x = 0;
    }
    if (y < 0) {
        y = 0;
    }
    if (x > width()) {
        x = width();
    }
    if (y > height()) {
        y = height();
    }
    return QPointF(x,y);
}

void Canvas::getInside(QGraphicsItem *item) const {
    QPointF corner1 = item->boundingRect().topLeft();
    QPointF corner2 = item->boundingRect().bottomRight();
    qreal x1 = item->x() -1;
    qreal y1 = item->y() -1;

    if (corner1.x() + x1 < 0) {
        item->setPos(-corner1.x(), item->pos().y());
    }
    if (corner1.y() + y1 < 0) {
        item->setPos(item->pos().x(), -corner1.y());
    }
    if (corner2.x() + x1 > width()) {
        item->setPos(width() - corner2.x(), item->pos().y());
    }
    if (corner2.y() + y1 > height()) {
        item->setPos(item->pos().x(), height() - corner2.y());
    }
}

bool Canvas::isInside(QPointF point) const {
    qreal x = point.x();
    qreal y = point.y();
    return x > 0 && y > 0 && x < width() && y < height();
}

void Canvas::pasteItem(QGraphicsItem *itemCopy) {
    QGraphicsItem *res = copyItem(itemCopy);
    addItem(res);
    QGraphicsView *v = views().back();
    QPoint p = v->mapFromGlobal(QCursor::pos());
    QPointF point = v->mapToScene(p);
    qDebug() << point << QCursor::pos();
    if (isInside(point)) {

        if (res->type() == FlowLine::Type) {
            FlowLine *line = static_cast<FlowLine*>(res);
            point -= (line->line().p1() + line->line().p2())/2;
        }

        res->setPos(point);
    }
    else {
        res->setPos(itemCopy->pos());
    }
    getInside(res);
    addToHistory(new InsertDeleteCommand(res, this, true));
}

void Canvas::setFont(const QFont &font) {
    textFont = font;
    for (auto & i : selectedItems()) {
        if (i->type() == TextField::Type) {
            TextField *textField = qgraphicsitem_cast<TextField *>(
                    selectedItems().first());
            textField->setFont(textFont);
        }
        break;
    }
}

void Canvas::resize(int w, int h) {
    clear();
    setSceneRect(0, 0, w, h);
    addLine(0, 0, width(), 0,QPen(QBrush(Qt::blue),2));
    addLine(width(), 0, width(), height(), QPen(QBrush(Qt::blue),2));
    addLine(width(), height(), 0, height(),QPen(QBrush(Qt::blue),2));
    addLine(0, height(), 0, 0, QPen(QBrush(Qt::blue),2));
}

void Canvas::penWidthChanged(int width) {
    itemPen.setWidth(width);
    borderButtonClicked();
}

void Canvas::penColorChanged(QColor color) {
    itemPen.setColor(color);
    borderButtonClicked();
}

void Canvas::borderButtonClicked() {
    for (auto &i : selectedItems()) {
        if (i->type() == FlowPolygon::Type) {
            static_cast<FlowPolygon*>(i)->setPen(itemPen);
        }
        else if (i->type() == FlowLine::Type) {
            if (itemPen.color() != Qt::white) {
                static_cast<FlowLine*>(i)->setPen(itemPen);
            }
        }
    }
}

void Canvas::save(const QString &file) {
    std::ofstream out{file.toUtf8().constData()};
    if (out.is_open() == false) {
        throw std::runtime_error("Cannot open file");
    }

    modified = false;
    print(out, "width", width(), "height", height());
    for (auto & item : items()) {
        if (item->type() == FlowLine::Type) {
            static_cast<FlowLine*>(item)->serialize(out);
        }
        else if (item->type() == TextField::Type) {
            static_cast<TextField*>(item)->serialize(out);
        }
        else if (item->type() == FlowPolygon::Type) {
            static_cast<FlowPolygon*>(item)->serialize(out);
        }
    }

    out.close();
}

QGraphicsItem *Canvas::loadItem(
        const std::string &itemStr, const std::string &data)
{
    std::istringstream ss{data};
    QGraphicsItem *item;
    if (itemStr == "FlowPolygon") {
        item = new FlowPolygon(ss);
    }
    else if (itemStr == "FlowLine") {
        item = new FlowLine(ss);
    }
    else if (itemStr == "TextField") {
        item = new TextField(ss);
        connect(static_cast<TextField*>(item), SIGNAL(lostFocus(TextField*)),
                this, SLOT(editorLostFocus(TextField*)));
        connect(static_cast<TextField*>(item),
                SIGNAL(selectedChange(QGraphicsItem*)), this,
                SIGNAL(itemSelected(QGraphicsItem*)));
    }
    else {
        qDebug() << QString(itemStr.c_str());
        assert(false);
    }

    return item;
}

void Canvas::load(const QString &file) {
    std::ifstream input{file.toUtf8().constData()};
    if (input.is_open() == false) {
        throw std::runtime_error("Cannot open file");
    }
    clearHistory();
    std::string buf;
    std::string databuf, curItem;
    int width, height;
    int state = 0;
    QList<QGraphicsItem*> itemList;

    while (getline(input, buf)) {
        std::istringstream ss(buf);
        std::string first; ss >> first;
        switch(state) {
            case 0:
                if (first == "width") {
                    ss >> width >> first >> height;
                    state = 1;
                }
                else {
                    throw std::runtime_error("syntax: width required");
                }

                break;
            case 1:
                if (first != "FlowPolygon" && first != "TextField" &&
                    first != "FlowLine")
                {
                    throw std::runtime_error("syntax: item keyword");
                }
                state = 2;
                curItem = first;
                break;
            case 2:
                if (first != "FlowPolygon" && first != "TextField" &&
                    first != "FlowLine")
                {
                    // read till next item
                    databuf += buf + "\n";
                }
                else {
                    // create item
                    itemList.append(loadItem(curItem, databuf));
                    databuf.clear();
                    curItem = first;
                }
                break;
            default:
                throw std::runtime_error("invalid syntax!");
        }
    }

    if (databuf != "") {
        itemList.append(loadItem(curItem, databuf));
    }

    resize(width, height);
    for (auto &i : itemList) {
        addItem(i);
    }
    modified = false;
    input.close();
}

void Canvas::undo() {
    if (!commands.isEmpty()) {
        qDebug() << "undoing";
        commands.undo();
        // disable button
        if (commands.isEmpty()) {
            static_cast<MainWindow*>(parent())->updateUndo(false);
        }
    }
}

void Canvas::remove() {
    for (auto &i : selectedItems()) {
        QGraphicsItem *item = copyItem(i);
        item->setPos(i->pos());
        addToHistory(new InsertDeleteCommand(item, this));
        removeItem(i);
        break;
    }
}

QGraphicsItem *Canvas::copyItem(QGraphicsItem *itemCopy) {
    QGraphicsItem *res;
    if (itemCopy->type() == FlowPolygon::Type) {
        res = new FlowPolygon(static_cast<FlowPolygon*>(itemCopy));
    }
    else if (itemCopy->type() == FlowLine::Type) {
        res = new FlowLine(static_cast<FlowLine*>(itemCopy));
    }
    else if (itemCopy->type() == TextField::Type) {
        res = new TextField(static_cast<TextField*>(itemCopy));
        connect(static_cast<TextField*>(res), SIGNAL(lostFocus(TextField*)),
                this, SLOT(editorLostFocus(TextField*)));
        connect(static_cast<TextField*>(res),
                SIGNAL(selectedChange(QGraphicsItem*)), this,
                SIGNAL(itemSelected(QGraphicsItem*)));
    }
    return res;
}

void Canvas::addToHistory(Command *cmd) {
    commands.add(cmd);
    static_cast<MainWindow*>(parent())->updateUndo(true);
}

void Canvas::clearHistory() {
    commands.clear();
    static_cast<MainWindow*>(parent())->updateUndo(false);
}
