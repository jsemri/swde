#ifndef DIAGRAMNODE_H
#define DIAGRAMNODE_H

#include <QGraphicsPolygonItem>

class FlowChartItem : public QGraphicsPolygonItem
{
public:
    enum {Type = UserType + 36};
    enum FlowChartItemType {Process, Condition, None};

private:
    FlowChartItemType node_type;
    QPolygonF polyg;
    QMenu *context_menu;
    QPointF pointer_pos;

public:
    FlowChartItem
    (FlowChartItemType type, QMenu *context_menu, QGraphicsItem *parent = 0);

    FlowChartItemType get_type() const { return node_type; }
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

signals:

public slots:
};

#endif // DIAGRAMNODE_H
