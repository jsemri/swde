#ifndef DIAGRAMNODE_H
#define DIAGRAMNODE_H

#include <QGraphicsPolygonItem>

class DiagramNode : public QGraphicsPolygonItem
{
public:
    enum {Type = UserType + 36};
    enum DiagramNodeType {Step, Condition, None};

private:
    DiagramNodeType node_type;
    QPolygonF polyg;
    QMenu *context_menu;

public:
    DiagramNode
    (DiagramNodeType type, QMenu *context_menu, QGraphicsItem *parent = 0);

    DiagramNodeType get_type() const { return node_type; }
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

#endif // DIAGRAMNODE_H
