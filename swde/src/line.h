#ifndef ARROW_H
#define ARROW_H

#include <QGraphicsLineItem>

class QGraphicsPolygonItem;
class QGraphicsLineItem;
class QGraphicsScene;
class QRectF;
class QGraphicsSceneMouseEvent;
class QPainterPath;

class Arrow : public QGraphicsLineItem
{
public:
    enum { Type = UserType + 21 };

    Arrow(QGraphicsItem *parent = 0);

    int type() const override { return Type; }
    QRectF boundingRect() const override;
    QPainterPath shape() const override;

protected:
    void paint(
            QPainter *painter, const QStyleOptionGraphicsItem *option,
            QWidget *widget) override;

private:
    QPolygonF arrowHead;
};

#endif // ARROW_H
