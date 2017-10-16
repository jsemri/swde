#ifndef FLOWCHARTITEM_H
#define FLOWCHARTITEM_H

#include <QGraphicsPolygonItem>
#include "flowitem.h"

class FlowPolygon : public QGraphicsPolygonItem, public FlowItem
{
private:
    FlowItem::Type itemType;
    QPolygonF polyg;
    qreal size;
    QMenu *contextMenu;

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

    QVariant
    itemChange(GraphicsItemChange change, const QVariant &value) override;

public:
    // identification used for graphic scene
    enum {Type = UserType + 36};

    FlowPolygon
    (FlowItem::Type type, QColor color, qreal size, QMenu *contextMenu,
     QGraphicsItem *parent = 0);
    ~FlowPolygon() {}

    QPolygonF polygon() const { return polyg; }
    int type() const override { return Type; }
    QPixmap image() const override;
    void changeColor(QColor color);
    void changeSize(qreal xratio, qreal yratio);
};

#endif // FLOWCHARTITEM_H
