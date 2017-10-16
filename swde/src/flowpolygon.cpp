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
    switch (itemType) {
        case FlowItem::Type::Decision:
            polyg << QPointF(-50, 0)
                  << QPointF(0, 50)
                  << QPointF(50, 0)
                  << QPointF(0, -50)
                  << QPointF(-50, 0);
            break;
        case FlowItem::Type::Process:
            polyg << QPointF(-40, -40)
                  << QPointF(40, -40)
                  << QPointF(40, 40)
                  << QPointF(-40, 40)
                  << QPointF(-40, -40);
            break;
        case FlowItem::Type::Terminal:
            path.addRoundedRect(-40, -30, 80,
                                60, 30, 30);
            polyg = path.toFillPolygon();
            break;
        case FlowItem::Type::IO:
            polyg << QPointF(60, -40)
                  << QPointF(35, 40)
                  << QPointF(-60, 40)
                  << QPointF(-35, -40)
                  << QPointF(60, -40);
            break;
        case FlowItem::Type::Circle:
            path.addEllipse(-40, -40, 80, 80);
            polyg = path.toFillPolygon();
            break;
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
    QPixmap pixmap(150, 150);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 8));
    painter.translate(75, 75);
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

void FlowPolygon::changeSize(qreal xratio, qreal yratio) {
    QPolygonF newPolyg;
    for (auto & point : polyg) {
        newPolyg.append(QPointF(point.x()*xratio, point.y()*yratio));
    }
    polyg = newPolyg;
    setPolygon(polyg);
}
