#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QWidget>
#include <QRect>
#include <QDebug>
#include "debug.h"
#include "canvas.h"

Canvas::Canvas(QMenu *node_menu, QWidget *parrent) :
    QGraphicsScene{parrent}, node_menu{node_menu}, node{0}, line{0},
    mode{Idle}, node_type{FlowChartItem::None}, active_item{nullptr}
{
    setSceneRect(QRectF(0, 0, 400, 400));
    setBackgroundBrush(Qt::white);
}


void Canvas::update_positions(const QRectF &rect) {
    Q_UNUSED(rect);
}

void Canvas::mousePressEvent(QGraphicsSceneMouseEvent *event) {

    ENTRY

    FlowChartItem *node;
    switch (mode) {
        case InsertNode:
            node = new FlowChartItem(node_type, node_menu);
            addItem(node);
            node->setPos(event->scenePos());
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
