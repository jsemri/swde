#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>
#include <QDebug>

#include "canvas.h"
#include "flowpolygon.h"

FlowPolygon::FlowPolygon
(FlowItem::Type type, QColor color, qreal size, QMenu *contextMenu,
 QGraphicsItem *parent) :
    QGraphicsPolygonItem(parent), FlowItem{}, itemType{type}, size{size},
    contextMenu{contextMenu}
{
    QPainterPath path;
    switch (type) {
        case FlowItem::Type::Decision:
            polyg << QPointF(-100*size, 0) << QPointF(0, 100*size)
                  << QPointF(100*size, 0) << QPointF(0, -100*size)
                  << QPointF(-100*size, 0);
            break;
        case FlowItem::Type::Process:
            polyg << QPointF(-100*size, -100*size)
                  << QPointF(100*size, -100*size)
                  << QPointF(100*size, 100*size)
                  << QPointF(-100*size, 100*size)
                  << QPointF(-100*size, -100*size);
            break;
        case FlowItem::Type::Terminal:
            path.moveTo(200, 50);
            path.arcTo(150, 0, 50, 50, 0, 90);
            path.arcTo(50, 0, 50, 50, 90, 90);
            path.arcTo(50, 50, 50, 50, 180, 90);
            path.arcTo(150, 50, 50, 50, 270, 90);
            path.lineTo(200, 25);
            polyg = path.toFillPolygon();
            break;
        case FlowItem::Type::IO:
            polyg << QPointF(-120*size, -80*size) << QPointF(-70*size, 80*size)
                  << QPointF(120*size, 80*size) << QPointF(70*size, -80*size)
                  << QPointF(-120*size, -80*size);
        default:
            ;
    }
    setPolygon(polyg);
    changeColor(color);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
}


QPixmap FlowPolygon::image() const
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

void FlowPolygon::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    qDebug() << "context menu event";
    return;
    // right click ~-~-~-> edit
    scene()->clearSelection();
    setSelected(true);
    contextMenu->exec(event->screenPos());
}

QVariant
FlowPolygon::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {

    }

    return value;
}

void FlowPolygon::changeColor(QColor color) {
    // change color and set brush gradient
    QRadialGradient gradient(QPointF(0,0), 250*size);
    gradient.setColorAt(0, color);
    gradient.setColorAt(1, Qt::white);
    QBrush brush(gradient);
    setBrush(brush);
}
