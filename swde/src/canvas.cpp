#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QWidget>
#include <QRect>
#include <QDebug>
#include "debug.h"
#include "canvas.h"

Canvas::Canvas(QMenu *item_menu, QWidget *parrent) :
    QGraphicsScene{parrent}, item_menu{item_menu}, item{0}, line{0},
    mode{MoveItem}, item_type{FlowChartItem::None}, item_color{Qt::white},
    active_item{nullptr}
{
    setBackgroundBrush(Qt::white);
}

void Canvas::mousePressEvent(QGraphicsSceneMouseEvent *event) {

    ENTRY

    FlowChartItem *item;
    switch (mode) {
        case InsertItem:
            item = new FlowChartItem(item_type, item_color, 0.5, item_menu);
            addItem(item);
            item->setPos(event->scenePos());
            break;
        case MoveItem:
            active_item = itemAt(event->scenePos(), QTransform());
            QGraphicsScene::mousePressEvent(event);
        default:
            break;
    };
}

void Canvas::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {

    ENTRY

    if (mode == MoveItem) {
        if (active_item) {
            QGraphicsScene::mouseMoveEvent(event);
        }
    }
}

void Canvas::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {

    ENTRY

    if (mode == MoveItem) {
        QGraphicsScene::mouseReleaseEvent(event);
        if (active_item) {
            // set item back
            static_cast<FlowChartItem*>(active_item)->set_old_zvalue();
            active_item = nullptr;
        }
    }
}


void Canvas::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {

    ENTRY

    Q_UNUSED(event);
}
