#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QWidget>
#include <QRect>
#include "canvas.h"

Canvas::Canvas(QMenu *node_menu, QWidget *parrent) :
    QGraphicsScene{parrent}, node_menu{node_menu}, node{0}, line{0},
    mode{Idle}, node_type{DiagramNode::None}
{
    setSceneRect(QRectF(0, 0, 400, 400));
    setBackgroundBrush(Qt::white);
}


void Canvas::update_positions(const QRectF &rect) {
    Q_UNUSED(rect);
}

void Canvas::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    DiagramNode *node;
    switch (mode) {
        case InsertNode:
            node = new DiagramNode(node_type, node_menu);
            addItem(node);
            node->setPos(event->scenePos());
            break;
        default:
            break;
    };
}

void Canvas::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    Q_UNUSED(event);
}

void Canvas::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    Q_UNUSED(event);
}

void Canvas::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    Q_UNUSED(event);
}
