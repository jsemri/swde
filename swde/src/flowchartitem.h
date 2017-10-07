#ifndef FLOWCHARTITEM_H
#define FLOWCHARTITEM_H

#include <QGraphicsPolygonItem>

class FlowChartItem : public QGraphicsPolygonItem
{
public:
    // identification used for graphic scene
    enum {Type = UserType + 36};
    // possible shapes
    enum FlowChartItemType {Process, Condition, None};

private:
    FlowChartItemType item_type;
    QPolygonF polyg;
    QMenu *context_menu;
    QPointF pointer_pos;

public:
    FlowChartItem
    (FlowChartItemType type, QColor color, QMenu *context_menu,
     QGraphicsItem *parent = 0);

    FlowChartItemType get_type() const { return item_type; }
    QPolygonF polygon() const { return polyg; }
    int type() const override { return Type; }
    QPixmap image() const;

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

    QVariant
    itemChange(GraphicsItemChange change, const QVariant &value) override;

    void move_top();

public:
    void change_color(QColor color) { setBrush(QBrush(color));}

signals:

public slots:
};

#endif // FLOWCHARTITEM_H
