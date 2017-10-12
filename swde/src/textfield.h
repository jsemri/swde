#ifndef TEXTFIELD_H
#define TEXTFIELD_H

#include <QPen>
#include <QGraphicsTextItem>
#include <QPixmap>

#include "flowitem.h"

class QGraphicsScene;
class QGraphicsItem;
class QGraphicsSceneMouseEvent;

class TextField : public QGraphicsTextItem, public FlowItem
{
    Q_OBJECT

public:
    enum {Type = UserType + 99};

    TextField(QGraphicsItem *parent = 0);
    int type() const override {return Type;}
    QPixmap image() const override;

signals:
    void lostFocus(TextField *textField);
    void selectedChange(QGraphicsItem *item);

private:
    QVariant
    itemChange(GraphicsItemChange change, const QVariant &value) override;
    void focusOutEvent(QFocusEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // TEXTFIELD_H
