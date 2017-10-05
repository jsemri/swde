#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>

#include "diagramnode.h"

DiagramNode::DiagramNode
(DiagramNodeType type, QMenu *context_menu, QGraphicsItem *parent) :
    QGraphicsPolygonItem(parent), node_type{type}, context_menu{context_menu}
{
    switch (type) {
        case Condition:
            polyg << QPointF(-100, 0) << QPointF(0, 100)
                  << QPointF(100, 0) << QPointF(0, -100)
                  << QPointF(-100, 0);
            break;
        case Step:
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

QPixmap DiagramNode::image() const
{
    QPixmap pixmap(250, 250);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 8));
    painter.translate(125, 125);
    painter.drawPolyline(polyg);

    return pixmap;
}

void DiagramNode::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    context_menu->exec(event->screenPos());
}

QVariant
DiagramNode::itemChange(GraphicsItemChange change, const QVariant &value)
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