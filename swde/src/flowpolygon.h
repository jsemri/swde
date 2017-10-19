#ifndef FLOWCHARTITEM_H
#define FLOWCHARTITEM_H

#include <QGraphicsPolygonItem>
#include <QPen>
#include <sstream>
#include "flowitem.h"

class FlowPolygon : public QGraphicsPolygonItem, public FlowItem
{
private:
    FlowItem::Type itemType;
    QPolygonF polyg;
    QColor color;

    static QPolygonF buildPolygon(FlowItem::Type itemType);

public:
    // identification used for graphic scene
    enum {Type = UserType + 36};

    FlowPolygon(
            FlowItem::Type type, QColor color = Qt::white,
            QPen pen = QPen(Qt::black, 1),
            QGraphicsItem *parent = 0);
    FlowPolygon(FlowPolygon *fpolyg);
    FlowPolygon(std::istringstream &data);

    ~FlowPolygon() {}

    QPolygonF polygon() const { return polyg; }
    int type() const override { return Type; }
    QPixmap image() const override;
    void serialize(std::ofstream &out) const override;
    void changeColor(QColor color);
    void changeSize(qreal xratio, qreal yratio);
};

#endif // FLOWCHARTITEM_H
