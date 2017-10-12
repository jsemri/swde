#include <QGraphicsSceneMouseEvent>
#include <QPainter>

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

QPixmap TextField::image() const {
    QPixmap pixmap(250, 250);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 8));
    QFont myFont;
    myFont.setPixelSize(100);
    painter.setFont(myFont);
    painter.drawText(20,125, QString("Text"));

    return pixmap;
}
