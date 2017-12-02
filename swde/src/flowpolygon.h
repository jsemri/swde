#ifndef FLOWCHARTITEM_H
#define FLOWCHARTITEM_H

#include <QGraphicsPolygonItem>
#include <QPen>
#include <sstream>
#include "flowitem.h"

// class for representing various nodes
class FlowPolygon : public QGraphicsPolygonItem, public FlowItem
{
private:
    // node type e.g. input, process, decision
    FlowItem::Type itemType;
    // return corresponding polygon to specific item type
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

    int type() const override { return Type; }
    QPixmap image() const override;
    void serialize(std::ofstream &out) const override;
    void changeColor(QColor color);
    void changeSize(qreal xratio, qreal yratio);
};

#endif // FLOWCHARTITEM_H
