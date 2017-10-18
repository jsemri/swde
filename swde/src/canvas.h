#ifndef CANVAS_H
#define CANVAS_H

#include <QGraphicsScene>
#include <QWidget>
#include <QList>
#include <QFont>

#include "flowpolygon.h"
#include "flowitem.h"

class Qmenu;
class TextField;
class QGraphicsTextItem;
class FlowLine;

class Canvas : public QGraphicsScene
{
    Q_OBJECT
public:
    // states of canvas
    enum Mode {InsertItem, InsertLine, InsertText, MoveItem, MoveLineP1,
               MoveLineP2, ResizeItem, Idle};
private:
    QMenu *itemMenu;

    Mode mode;
    FlowItem::Type itemType;

    QGraphicsItem *activeItem;
    QPointF startArrowPoint;
    bool startArrowPointSet;
    FlowLine *arrow;
    QColor itemColor;
    QPen itemPen;

    void getInside(QGraphicsItem *item) const;
    QPointF getInside(QPointF point) const;
    bool isInside(QPointF point) const;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

public:
    explicit Canvas(QMenu *itemMenu, QWidget *parrent=0);
    void pasteItem(QGraphicsItem *itemCopy);
    void resize(int width, int h);
    void setItemColor(QColor color) { itemColor = color; }
    QPen pen() const { return itemPen; }

public slots:
    void setMode(Mode m = MoveItem) { mode = m; }
    void setItemType(FlowItem::Type type) { itemType = type; }
    void editorLostFocus(TextField *item);
    void penWidthChanged(int width);
    void penColorChanged(QColor color);
    void borderButtonClicked();

signals:
    void textInserted(QGraphicsTextItem *item);
    void itemSelected(QGraphicsItem *item);
    void itemInserted(FlowPolygon *item);
    void arrowInserted();
};

#endif // CANVAS_H
