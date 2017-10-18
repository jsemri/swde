#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QWidget>
#include <QRect>
#include <QDebug>
#include <QTextCursor>
#include <QGraphicsView>
#include <math.h>
#include <cassert>

#include "mainwindow.h"
#include "flowline.h"
#include "canvas.h"
#include "textfield.h"

Canvas::Canvas(QMenu *itemMenu, QWidget *parrent) :
    QGraphicsScene{parrent}, itemMenu{itemMenu},
    mode{MoveItem}, itemType{FlowItem::Type::None},
    activeItem{nullptr}, itemColor(Qt::white), itemPen{QPen(Qt::black, 1)}
{
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
            getInside(item);
            emit itemInserted(item);
            break;
        case MoveItem:
            // resize action
            for (auto & it : selectedItems()) {
                if (it->type() == FlowPolygon::Type) {
                    qreal x = event->scenePos().x();
                    qreal y = event->scenePos().y();
                    QPointF corner = it->boundingRect().bottomRight();
                    qreal cx = corner.x() + activeItem->x();
                    qreal cy = corner.y() + activeItem->y();
                    if ( cx > x && cx - x < 20 && cy > y && cy - y < 20)
                    {
                        qDebug() << "switched to resize mode";
                        mode = ResizeItem;
                        activeItem = it;
                        return;
                    }
                }
            }
            qDebug() << "moving item";
            activeItem = itemAt(event->scenePos(), QTransform());
            if (activeItem && activeItem->type() == FlowLine::Type) {
                // moving arrow
                arrow = static_cast<FlowLine*>(activeItem);
                qreal x = event->scenePos().x();
                qreal y = event->scenePos().y();
                if (fabs(arrow->line().p1().x() + arrow->x() - x) < 15 &&
                    fabs(arrow->line().p1().y() + arrow->y() - y) < 15)
                {
                    mode = MoveLineP1;
                }
                else if (fabs(arrow->line().p2().x() + arrow->x() - x) < 15 &&
                         fabs(arrow->line().p2().y() + arrow->y() - y) < 15)
                {
                    mode = MoveLineP2;
                }
            }

            QGraphicsScene::mousePressEvent(event);
            break;
        case InsertLine:
            qDebug() << "inserting line";
            arrow = new FlowLine(itemType == FlowItem::Type::Arrow,
                                 itemPen,
                                 event->scenePos(),event->scenePos());
            addItem(arrow);
            break;
        case InsertText:
            qDebug() << "inserting text";
            text = new TextField();
            text->setZValue(1001);
            //text->setFont(QFont::Normal);
            text->setTextInteractionFlags(Qt::TextEditorInteraction);
            addItem(text);
            text->setPos(event->scenePos());
            text->setDefaultTextColor(Qt::black);
            connect(text, SIGNAL(lostFocus(TextField*)), this,
                    SLOT(editorLostFocus(TextField*)));
            connect(text, SIGNAL(selectedChange(QGraphicsItem*)), this,
                    SIGNAL(itemSelected(QGraphicsItem*)));
            emit textInserted(text);
            QGraphicsScene::mousePressEvent(event);
            break;
        default:
            qDebug() << "undefined operation";
            QGraphicsScene::mousePressEvent(event);
    };

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
                /*
                XXX this also works, but keeps position, what is not desired
                QMatrix m;
                m.scale(xratio, yratio);
                activeItem->setTransform(QTransform(m));
                */

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
    QGraphicsItem *res;
    if (itemCopy->type() == FlowPolygon::Type) {
        res = new FlowPolygon(static_cast<FlowPolygon*>(itemCopy));
    }
    else if (itemCopy->type() == FlowLine::Type) {
        res = new FlowLine(static_cast<FlowLine*>(itemCopy));
    }
    else if (itemCopy->type() == TextField::Type) {
        res = new TextField(static_cast<TextField*>(itemCopy));
    }

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
