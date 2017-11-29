#include <QDebug>
#include <QGraphicsScene>

#include "commands.h"
#include "flowline.h"

// abstract class command constructor

Command::Command(QGraphicsItem *item) : item{item}
{

}

// different commands implementations

// item
MoveCommand::MoveCommand(QGraphicsItem *item, qreal posX, qreal posY) :
    Command{item}, posX{posX}, posY{posY}, linepoint{false}, p2{false}
{

}

// line
MoveCommand::MoveCommand(QGraphicsItem *item, QPointF point, bool isp2) :
    Command{item}, posX{point.x()}, posY{point.y()}, linepoint{true}, p2{isp2}
{

}

void MoveCommand::undo() {
    if (linepoint) {
        if (p2) {
            static_cast<FlowLine*>(item)->setBeginPoint(QPointF(posX, posY));
        }
        else {
            static_cast<FlowLine*>(item)->setEndPoint(QPointF(posX, posY));
        }
    }
    else {
        item->setPos(posX, posY);
    }
    item->update();
}

InsertDeleteCommand::InsertDeleteCommand(
        QGraphicsItem *item, QGraphicsScene *scene, bool toDelete) :
    Command{item}, scene{scene}, toDelete{toDelete}
{

}

void InsertDeleteCommand::undo() {
    if (item) {
        if (toDelete) {
            scene->removeItem(item);
        }
        else {
            scene->addItem(item);
        }
    }
}
