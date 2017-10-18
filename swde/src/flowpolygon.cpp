#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>
#include <QDebug>

#include "canvas.h"
#include "flowpolygon.h"

FlowPolygon::FlowPolygon
(FlowItem::Type type, QColor color, QPen pen, QGraphicsItem *parent) :
    QGraphicsPolygonItem(parent), FlowItem{}, itemType{type}
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
    setPen(pen);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
}


FlowPolygon::FlowPolygon(FlowPolygon *fpolyg) :
    FlowPolygon{fpolyg->itemType, fpolyg->brush().color(),
                fpolyg->pen()}
{
    polyg = fpolyg->polygon();
    setPolygon(polyg);
    setBrush(fpolyg->brush());
    setPos(pos());
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

void FlowPolygon::changeColor(QColor color) {
    // change color and set brush gradient
    QLinearGradient gradient(0,0,0,45);
    gradient.setColorAt(0, color);
    gradient.setColorAt(1, Qt::white);
    setBrush(gradient);
}

void FlowPolygon::changeSize(qreal xratio, qreal yratio) {
    xratio += 0.0001;
    yratio += 0.0001;
    QPolygonF newPolyg;
    for (auto & point : polyg) {
        newPolyg.append(QPointF(point.x()*xratio, point.y()*yratio));
    }
    polyg = newPolyg;
    setPolygon(polyg);
}
