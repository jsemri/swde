#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QWidget>
#include <QRect>
#include <QDebug>
#include <QTextCursor>
#include <math.h>
#include <cassert>

#include "flowline.h"
#include "canvas.h"
#include "textfield.h"

Canvas::Canvas(QMenu *itemMenu, QWidget *parrent) :
    QGraphicsScene{parrent}, itemMenu{itemMenu},
    mode{MoveItem}, itemType{FlowItem::Type::None},
    activeItem{nullptr}
{
    setBackgroundBrush(Qt::white);
}

void Canvas::mousePressEvent(QGraphicsSceneMouseEvent *event) {

    FlowPolygon *item;
    TextField *text;
    switch (mode) {
        case InsertItem:
            qDebug() << "inserting item";
            item = new FlowPolygon(itemType, Qt::white, 0.5, itemMenu);
            addItem(item);
            item->setPos(event->scenePos());
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
                if (fabs(arrow->line().p1().x() + arrow->x() - x) < 10)
                {
                    mode = MoveLineP1;
                }
                else if (fabs(arrow->line().p2().x() + arrow->x() - x)
                         < 10)
                {
                    mode = MoveLineP2;
                }
            }

            QGraphicsScene::mousePressEvent(event);
            break;
        case InsertLine:
            qDebug() << "inserting line";
            arrow = new FlowLine(itemType == FlowItem::Type::Arrow,
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
            QGraphicsScene::mouseReleaseEvent(event);
            break;
        case MoveLineP2:
        case MoveLineP1:
            mode = MoveItem;
        case InsertLine:
            arrow = 0;
        case MoveItem:
            QGraphicsScene::mouseReleaseEvent(event);
        default:
            ;
    }
    //QGraphicsScene::mouseReleaseEvent(event);
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

QPointF Canvas::getInside(QPointF point) {
    qreal x = point.x();
    qreal y = point.y();
    if (x < 0) {
        x = 0;
    }
    if (y < 0) {
        y = 0;
    }
    if (x > 1000) {
        x = 1000;
    }
    if (y > 1000) {
        y = 1000;
    }
    return QPointF(x,y);
}

void Canvas::getInside(QGraphicsItem *item) {
    QPointF corner1 = item->boundingRect().topLeft();
    QPointF corner2 = item->boundingRect().bottomRight();
    qreal x1 = item->x() -1;
    qreal y1 = item->y() -1;
    qreal x2 = corner2.x() + item->x() + 1;
    qreal y2 = corner2.y() + item->y() + 1;

    if (corner1.x() + x1 < 0) {
        item->setPos(-corner1.x(), item->pos().y());

    }
    if (corner1.y() + y1 < 0) {
        item->setPos(item->pos().x(), -corner1.y());

    }
    if (corner2.x() + x2 > 1000) {
        item->setPos(-corner2.x(), item->pos().y());


    }
    if (corner2.y() + y2 > 1000) {
        item->setPos(item->pos().x(), -corner2.y());

    }
}
