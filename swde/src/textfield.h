#ifndef TEXTFIELD_H
#define TEXTFIELD_H

#include <QPen>
#include <QGraphicsTextItem>

class QGraphicsScene;
class QGraphicsItem;
class QGraphicsSceneMouseEvent;

class TextField : public QGraphicsTextItem
{
    Q_OBJECT

public:
    enum {Type = UserType + 99};

    TextField(QGraphicsItem *parent = 0);
    int type() const override {return Type;}
signals:
    void lostFocus(TextField *text_field);
    void selectedChange(QGraphicsItem *item);

private:
    QVariant
    itemChange(GraphicsItemChange change, const QVariant &value) override;
    void focusOutEvent(QFocusEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // TEXTFIELD_H
