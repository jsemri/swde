#include <QGraphicsSceneMouseEvent>
#include "textfield.h"
#include "canvas.h"

TextField::TextField(QGraphicsItem *parent) :
    QGraphicsTextItem{parent}
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
}

QVariant
TextField::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == QGraphicsItem::ItemSelectedHasChanged) {
        emit selectedChange(this);
    }
    return value;
}

void TextField::focusOutEvent(QFocusEvent *event) {
    setTextInteractionFlags(Qt::NoTextInteraction);
    emit lostFocus(this);
    QGraphicsItem::focusOutEvent(event);
}

void TextField::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    //qDebug() << "Asd";
    if (textInteractionFlags() == Qt::NoTextInteraction) {
        setTextInteractionFlags(Qt::TextEditorInteraction);
    }
    QGraphicsTextItem::mouseDoubleClickEvent(event);
}
