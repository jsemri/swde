#ifndef FLOWCHARTITEM_H
#define FLOWCHARTITEM_H

#include <QGraphicsPolygonItem>

class FlowChartItem : public QGraphicsPolygonItem
{
public:
    // identification used for graphic scene
    enum {Type = UserType + 36};
    // possible shapes
    enum FlowChartItemType {Process, Condition, Text, Line, None};

private:
    FlowChartItemType itemType;
    QPolygonF polyg;
    qreal size;
    QMenu *contextMenu;

public:
    FlowChartItem
    (FlowChartItemType type, QColor color, qreal size, QMenu *contextMenu,
     QGraphicsItem *parent = 0);

    FlowChartItemType getType() const { return itemType; }
    QPolygonF polygon() const { return polyg; }
    int type() const override { return Type; }
    QPixmap image() const;

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

    QVariant
    itemChange(GraphicsItemChange change, const QVariant &value) override;

public:
    void changeColor(QColor color);
};

#endif // FLOWCHARTITEM_H
