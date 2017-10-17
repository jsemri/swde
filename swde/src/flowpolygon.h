#ifndef FLOWCHARTITEM_H
#define FLOWCHARTITEM_H

#include <QGraphicsPolygonItem>
#include "flowitem.h"

class FlowPolygon : public QGraphicsPolygonItem, public FlowItem
{
private:
    FlowItem::Type itemType;
    QPolygonF polyg;

public:
    // identification used for graphic scene
    enum {Type = UserType + 36};

    FlowPolygon
    (FlowItem::Type type, QBrush brush, QGraphicsItem *parent = 0);
    FlowPolygon(FlowPolygon *fpolyg);
    ~FlowPolygon() {}

    QPolygonF polygon() const { return polyg; }
    int type() const override { return Type; }
    QPixmap image() const override;
    void changeBrush(QBrush brush);
    void changeSize(qreal xratio, qreal yratio);
};

#endif // FLOWCHARTITEM_H
