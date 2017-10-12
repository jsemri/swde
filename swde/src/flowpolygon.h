#ifndef FLOWCHARTITEM_H
#define FLOWCHARTITEM_H

#include <QGraphicsPolygonItem>
#include "flowitem.h"

class FlowPolygon : public QGraphicsPolygonItem, public FlowItem
{
public:
    // identification used for graphic scene
    enum {Type = UserType + 36};

private:
    FlowItem::Type itemType;
    QPolygonF polyg;
    qreal size;
    QMenu *contextMenu;

public:
    FlowPolygon
    (FlowItem::Type type, QColor color, qreal size, QMenu *contextMenu,
     QGraphicsItem *parent = 0);
    ~FlowPolygon() {}

    QPolygonF polygon() const { return polyg; }
    int type() const override { return Type; }
    QPixmap image() const override;

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

    QVariant
    itemChange(GraphicsItemChange change, const QVariant &value) override;

public:
    void changeColor(QColor color);
};

#endif // FLOWCHARTITEM_H
