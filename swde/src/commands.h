#ifndef COMMAND_H
#define COMMAND_H

#include <QGraphicsItem>

class QGraphicsScene;

// providing undo operaions using command design patter
class Command
{
protected:
    // item to change / add / delete
    QGraphicsItem *item;
public:
    Command(QGraphicsItem *item);
    virtual ~Command() {}
    virtual void undo() = 0;
    QGraphicsItem *getItem() const { return item;}
};

// different commands for different operations

class MoveCommand : public Command
{
protected:
    /// previous position
    qreal posX;
    qreal posY;
    bool linepoint;
    bool p2;

public:
    MoveCommand(QGraphicsItem *item, qreal posX, qreal posY);
    MoveCommand(QGraphicsItem *item, QPointF point, bool isp2 = false);
    ~MoveCommand() {}

    virtual void undo() override;
};

class InsertDeleteCommand : public Command
{
    QGraphicsScene *scene;
    bool toDelete;
public:
    InsertDeleteCommand(QGraphicsItem *item, QGraphicsScene *scene, bool toDelete = false);
    ~InsertDeleteCommand() {}

    virtual void undo() override;
};

class RatioChangeCommand : public Command
{
    QPolygonF polygon;
public:
    RatioChangeCommand(QGraphicsItem *item, QPolygonF polygon);
    ~RatioChangeCommand() {}

    virtual void undo() override;
};

#endif // COMMAND_H
