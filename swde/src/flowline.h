#ifndef ARROW_H
#define ARROW_H

#include <QGraphicsLineItem>
#include <QPixmap>

#include "flowitem.h"

class QGraphicsPolygonItem;
class QGraphicsLineItem;
class QGraphicsScene;
class QRectF;
class QGraphicsSceneMouseEvent;
class QPainterPath;

class FlowLine : public QGraphicsLineItem, public FlowItem
{
public:
    enum { Type = UserType + 21 };

    FlowLine(bool arrowSet = false, QPointF beginPoint = QPoint(),
             QPointF endPoint = QPoint(), QGraphicsItem *parent = 0);

    int type() const override { return Type; }
    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    void setBeginPoint(QPointF point) {
        setLine(QLineF(point-pos(), line().p2()));
    }

    void setEndPoint(QPointF point) {
        setLine(QLineF(line().p1(), point - pos()));
    }

    QPixmap image() const override;

protected:
    void paint(
            QPainter *painter, const QStyleOptionGraphicsItem *option,
            QWidget *widget) override;

private:
    bool arrowSet;
    QPolygonF arrowHead;
};

#endif // ARROW_H
