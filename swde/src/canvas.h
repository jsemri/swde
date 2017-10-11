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

class Canvas : public QGraphicsScene
{
    Q_OBJECT
public:
    // states of canvas
    enum Mode {InsertItem, InsertLine, InsertText, MoveItem, Idle};

private:
    QMenu *item_menu;

    Mode mode;
    FlowChartItem::FlowChartItemType item_type;
    QColor item_color;
    QColor border_color;
    QFont text_font;

    QGraphicsItem *active_item;

private:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;    

public:
    explicit Canvas(QMenu *item_menu, QWidget *parrent=0);

public slots:
    void set_mode(Mode m = MoveItem) { mode = m; }
    void set_color(QColor color) { item_color = color; }
    void set_border_color(QColor color) { border_color = color; }
    void set_item_type(FlowChartItem::FlowChartItemType type) { item_type = type; }
    void editorLostFocus(TextField *item);

signals:
    void textInserted(QGraphicsTextItem *item);
    void itemSelected(QGraphicsItem *item);
};

#endif // CANVAS_H
