#ifndef COMPONENT_H
#define COMPONENT_H

#include <QGraphicsPolygonItem>

class Component : public QGraphicsPolygonItem
{
public:
    enum {Type = UserType + 36};
    enum ComponentType {Step, Condition};

private:
    ComponentType comtype;
    QPolygonF polyg;
    QMenu *context_menu;

public:
    Component
    (ComponentType type, QMenu *context_menu, QGraphicsItem *parent = 0);

    ComponentType get_type() const { return comtype; }
    QPolygonF polygon() const { return polyg; }
    int type() const override { return Type; }
    QPixmap image() const;

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

    QVariant
    itemChange(GraphicsItemChange change, const QVariant &value) override;

signals:

public slots:
};

#endif // COMPONENT_H
