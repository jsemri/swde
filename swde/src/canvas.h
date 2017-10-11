#ifndef CANVAS_H
#define CANVAS_H

#include <QGraphicsScene>
#include <QWidget>
#include <QList>
#include <QFont>

#include "flowchartitem.h"

class Qmenu;
class TextField;
class QGraphicsTextItem;
class Arrow;

class Canvas : public QGraphicsScene
{
    Q_OBJECT
public:
    // states of canvas
    enum Mode {InsertItem, InsertLine, InsertText, MoveItem, Idle};

    explicit Canvas(QMenu *itemMenu, QWidget *parrent=0);

private:
    QMenu *itemMenu;

    Mode mode;
    FlowChartItem::FlowChartItemType itemType;

    QGraphicsItem *activeItem;
    QPointF startArrowPoint;
    bool startArrowPointSet;
    Arrow *arrow;


    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;    


public slots:
    void setMode(Mode m = MoveItem) { mode = m; }
    void setItemType(FlowChartItem::FlowChartItemType type) { itemType = type; }
    void editorLostFocus(TextField *item);

signals:
    void textInserted(QGraphicsTextItem *item);
    void itemSelected(QGraphicsItem *item);
    void itemInserted(FlowChartItem *item);
    void arrowInserted();
};

#endif // CANVAS_H
