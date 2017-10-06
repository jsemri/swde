#ifndef CANVAS_H
#define CANVAS_H

#include <QGraphicsScene>
//#include <QGraphicsItem>
#include <QWidget>
#include <QList>

#include "flowchartitem.h"

class Qmenu;

class Canvas : public QGraphicsScene
{
 //   Q_OBJECT
public:
    // states of canvas
    enum Mode {InsertItem, InsertLine, InsertText, MoveItem, Idle};

private:
    QMenu *item_menu;
    FlowChartItem *item;
    FlowChartItem *line;
    Mode mode;
    FlowChartItem::FlowChartItemType item_type;

    QGraphicsItem *active_item;

private:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

public:
    explicit Canvas(QMenu *item_menu, QWidget *parrent=0);

    void update_positions(const QRectF &rect);

public slots:
    void set_mode(Mode m) { mode = m; }
    void set_item_type(FlowChartItem::FlowChartItemType type) { item_type = type; }

signals:
};

#endif // CANVAS_H
