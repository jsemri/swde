#include <QGraphicsScene>
#include <QWidget>
#include <QRect>
#include "canvas.h"

Canvas::Canvas(QWidget *parrent) :
    QGraphicsScene{parrent}
{
    setSceneRect(QRectF(0,0,400,400));
    setBackgroundBrush(Qt::white);
}


void Canvas::update_positions(const QRectF &rect) {
    Q_UNUSED(rect);
}

void Canvas::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    Q_UNUSED(event);
}

void Canvas::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    Q_UNUSED(event);
}

void Canvas::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    Q_UNUSED(event);
}

void Canvas::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    Q_UNUSED(event);
}
