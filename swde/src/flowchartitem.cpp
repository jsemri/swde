#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>
#include <QDebug>

#include "debug.h"
#include "canvas.h"
#include "flowchartitem.h"

FlowChartItem::FlowChartItem
(FlowChartItemType type, QColor color, qreal size, QMenu *context_menu,
 QGraphicsItem *parent) :
    QGraphicsPolygonItem(parent), item_type{type}, size{size},
    context_menu{context_menu}, old_zvalue{0}
{
    switch (type) {
        case Condition:
            polyg << QPointF(-100*size, 0) << QPointF(0, 100*size)
                  << QPointF(100*size, 0) << QPointF(0, -100*size)
                  << QPointF(-100*size, 0);
            break;
        case Process:
            polyg << QPointF(-100*size, -100*size)
                  << QPointF(100*size, -100*size)
                  << QPointF(100*size, 100*size)
                  << QPointF(-100*size, 100*size)
                  << QPointF(-100*size, -100*size);
            break;
        default:
            polyg << QPointF(-120*size, -80*size) << QPointF(-70*size, 80*size)
                  << QPointF(120*size, 80*size) << QPointF(70*size, -80*size)
                  << QPointF(-120*size, -80*size);
            break;
    }
    setPolygon(polyg);
    change_color(color);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}


QPixmap FlowChartItem::image() const
{
    // just output the item image, used for creating buttons
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
    if (change == QGraphicsItem::ItemPositionChange) {
        /*
        foreach (Arrow *arrow, arrows) {
            arrow->updatePosition();
        }
        */
    }

    return value;
}

void FlowChartItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    ENTRY

    pointer_pos = event->scenePos();
    // move top
    set_zvalue(1000);
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

void FlowChartItem::change_color(QColor color) {
    // change color and set brush gradient
    QRadialGradient gradient(QPointF(0,0), 250*size);
    gradient.setColorAt(0, color);
    gradient.setColorAt(1, Qt::white);
    QBrush brush(gradient);
    setBrush(brush);
}

void FlowChartItem::change_size(qreal s) {
    size = s;
    change_color(this->brush().color());
}
