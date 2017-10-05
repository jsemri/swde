#ifndef CANVAS_H
#define CANVAS_H

#include <QGraphicsScene>
//#include <QGraphicsItem>
#include <QWidget>
#include <QList>

class Canvas : public QGraphicsScene
{
private:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

public:
    Canvas(QWidget *parrent=0);

    void update_positions(const QRectF &rect);
};

#endif // CANVAS_H
