#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QWidget>
#include <QRect>
#include <QDebug>
#include "debug.h"
#include "canvas.h"

Canvas::Canvas(QMenu *item_menu, QWidget *parrent) :
    QGraphicsScene{parrent}, item_menu{item_menu}, item{0}, line{0},
    mode{Idle}, item_type{FlowChartItem::None}, active_item{nullptr}
{
    setSceneRect(QRectF(0, 0, 400, 400));
    setBackgroundBrush(Qt::white);
}


void Canvas::update_positions(const QRectF &rect) {
    Q_UNUSED(rect);
}

void Canvas::mousePressEvent(QGraphicsSceneMouseEvent *event) {

    ENTRY

    FlowChartItem *item;
    switch (mode) {
        case InsertItem:
            item = new FlowChartItem(item_type, item_menu);
            addItem(item);
            item->setPos(event->scenePos());
            break;
        case Idle:
            active_item = itemAt(event->scenePos(), QTransform());
            QGraphicsScene::mousePressEvent(event);
            if (active_item) {

            }
        default:
            break;
    };
}

void Canvas::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {

    ENTRY

    if (mode == Idle) {
        if (active_item) {
            QGraphicsScene::mouseMoveEvent(event);
        }
    }
}

void Canvas::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {

    ENTRY

    if (mode == Idle) {
        QGraphicsScene::mouseReleaseEvent(event);
        if (active_item) {
            // set item back
            active_item->setZValue(0);
            active_item = nullptr;
        }
    }
}


void Canvas::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {

    ENTRY

    Q_UNUSED(event);
}
