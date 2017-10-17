#include <QPainter>
#include <QPen>
#include <QDebug>

#include <math.h>
#include "flowline.h"

const qreal Pi = 3.14;

FlowLine::FlowLine(bool arrowSet,
        QPointF beginPoint, QPointF endPoint,
        QGraphicsItem *parent) :
    QGraphicsLineItem(parent), arrowSet{arrowSet}
{
    setLine(QLineF(beginPoint, endPoint));
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}

FlowLine::FlowLine(FlowLine *fline) :
    FlowLine{fline->arrowSet, fline->line().p1(), fline->line().p2()}
{
    qDebug() << "line position: " << fline->pos();
    setPos(fline->pos());
    setPen(fline->pen());
}

QRectF FlowLine::boundingRect() const {
    qreal extra = (pen().width() + 10) / 2.0;
    return QRectF(line().p1(), line().p2()).normalized()
           .adjusted(-extra, -extra, extra, extra);
}

QPainterPath FlowLine::shape() const {
    QPainterPath path = QGraphicsLineItem::shape();
    if (arrowSet) {
        path.addPolygon(arrowHead);
    }
    return path;
}

void FlowLine::paint(
    QPainter *painter, const QStyleOptionGraphicsItem *option,
    QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    if (fabs(line().dx()) < 5 && fabs(line().dy()) < 5) {
        return;
    }

    QPen myPen = pen();
    myPen.setColor(Qt::black);
    painter->setPen(myPen);
    painter->setBrush(Qt::black);
    if (arrowSet) {
        double alpha = ::acos(line().dx() / (line().length()+0.00001));
        if (line().dy() <= 0) {
            alpha = (Pi * 2) - alpha;
        }
        double beta = Pi/12;
        int size = 15;
        QPointF p1, p2;
        p1 = line().p2() - QPointF(cos(alpha - beta) * size,
                                 sin(alpha - beta) * size);
        p2 = line().p2() - QPointF(sin(Pi/2 - alpha - beta) * size,
                                 cos(Pi/2 - alpha - beta) * size);
        arrowHead.clear();
        arrowHead << line().p2() << p1 << p2;
        painter->drawPolygon(arrowHead);
    }

    painter->drawLine(line());
}

QPixmap FlowLine::image() const {
    QPixmap pixmap(250, 250);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 8));
    if (arrowSet) {
        QPolygonF arrow;
        arrow << QPointF(240, 10) << QPointF(240, 50) << QPointF(200, 10);
        painter.drawPolygon(arrow);
    }
    painter.drawLine(10, 240, 240, 10);

    return pixmap;
}
