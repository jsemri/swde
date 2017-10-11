#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>
#include <QDebug>


#include "canvas.h"
#include "flowchartitem.h"

FlowChartItem::FlowChartItem
(FlowChartItemType type, QColor color, qreal size, QMenu *contextMenu,
 QGraphicsItem *parent) :
    QGraphicsPolygonItem(parent), itemType{type}, size{size},
    contextMenu{contextMenu}
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
    changeColor(color);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    //this->setSelected(false);
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
    contextMenu->exec(event->screenPos());
}

QVariant
FlowChartItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {

    }

    return value;
}

void FlowChartItem::changeColor(QColor color) {
    // change color and set brush gradient
    QRadialGradient gradient(QPointF(0,0), 250*size);
    gradient.setColorAt(0, color);
    gradient.setColorAt(1, Qt::white);
    QBrush brush(gradient);
    setBrush(brush);
}
