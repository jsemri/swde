#ifndef CANVAS_H
#define CANVAS_H

#include <QGraphicsScene>
//#include <QGraphicsItem>
#include <QWidget>
#include <QList>

#include "diagramnode.h"

class Qmenu;

class Canvas : public QGraphicsScene
{
 //   Q_OBJECT
public:
    enum Mode {InsertNode, InsertLine, InsertTetx, MoveNode, Idle};
private:
    QMenu *node_menu;
    DiagramNode *node;
    DiagramNode *line;
    Mode mode;
    DiagramNode::DiagramNodeType node_type;

private:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

public:
    explicit Canvas(QMenu *node_menu, QWidget *parrent=0);

    void update_positions(const QRectF &rect);

public slots:
    void set_mode(Mode m) { mode = m; }
    void set_node_type(DiagramNode::DiagramNodeType type) { node_type = type; }

signals:
};

#endif // CANVAS_H
