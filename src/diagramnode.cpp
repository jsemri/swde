#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>
#include <QDebug>

#include "debug.h"
#include "canvas.h"
#include "diagramnode.h"

FlowChartItem::FlowChartItem
(FlowChartItemType type, QMenu *context_menu, QGraphicsItem *parent) :
    QGraphicsPolygonItem(parent), node_type{type}, context_menu{context_menu}
{
    switch (type) {
        case Condition:
            polyg << QPointF(-100, 0) << QPointF(0, 100)
                  << QPointF(100, 0) << QPointF(0, -100)
                  << QPointF(-100, 0);
            break;
        case Process:
            polyg << QPointF(-100, -100) << QPointF(100, -100)
                  << QPointF(100, 100) << QPointF(-100, 100)
                  << QPointF(-100, -100);
            break;
        default:
            polyg << QPointF(-120, -80) << QPointF(-70, 80)
                  << QPointF(120, 80) << QPointF(70, -80)
                  << QPointF(-120, -80);
            break;
    }
    setPolygon(polyg);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

QPixmap FlowChartItem::image() const
{
    QPixmap pixmap(250, 250);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 8));
    painter.translate(125, 125);
    painter.drawPolyline(polyg);

    return pixmap;
}

void FlowChartItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    context_menu->exec(event->screenPos());
}

QVariant
FlowChartItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    ENTRY

    if (change == QGraphicsItem::ItemPositionChange) {
        /*
        foreach (Arrow *arrow, arrows) {
            arrow->updatePosition();
        }
        */
    }

    return value;
}

void FlowChartItem::move_top() {
    setZValue(1000);
}

void FlowChartItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    ENTRY

    pointer_pos = event->scenePos();
    move_top();
}

void FlowChartItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    ENTRY
    // move only if selected
    if (isSelected()) {
        QPointF current_pos = event->scenePos();
        qreal xdiff = current_pos.x() - pointer_pos.x();
        qreal ydiff = current_pos.y() - pointer_pos.y();
        moveBy(xdiff, ydiff);
        pointer_pos = current_pos;
    }
}
