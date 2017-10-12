#include <QPainter>
#include <QPen>
#include <QDebug>

#include "flowline.h"

FlowLine::FlowLine(
    QPointF beginPoint, QPointF endPoint, QGraphicsItem *parent) :
    QGraphicsLineItem(parent), beginPoint{beginPoint}, endPoint{endPoint}
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}

QRectF FlowLine::boundingRect() const {
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

QPainterPath FlowLine::shape() const {
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(arrowHead);
    return path;
}

void FlowLine::paint(
    QPainter *painter, const QStyleOptionGraphicsItem *option,
    QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    QPen myPen = pen();
    myPen.setColor(Qt::black);
    //qreal arrowSize = 20;
    painter->setPen(myPen);
    painter->setBrush(Qt::black);

    setLine(QLineF(beginPoint, endPoint));
    painter->drawLine(line());
}

QPixmap FlowLine::image() const {
    QPixmap pixmap(250, 250);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 8));
    painter.drawLine(10, 240, 240, 10);

    return pixmap;
}
