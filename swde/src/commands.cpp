#include <QDebug>
#include <QGraphicsScene>

#include "commands.h"
#include "flowline.h"
#include "flowpolygon.h"

// abstract class command constructor

Command::Command(QGraphicsItem *item) : item{item}
{

}

// different commands implementations

// moving the item
MoveCommand::MoveCommand(QGraphicsItem *item, qreal posX, qreal posY) :
    Command{item}, posX{posX}, posY{posY}, linepoint{false}, p2{false}
{

}

// moving the line
MoveCommand::MoveCommand(QGraphicsItem *item, QPointF point, bool isp2) :
    Command{item}, posX{point.x()}, posY{point.y()}, linepoint{true}, p2{isp2}
{

}

void MoveCommand::undo() {
    if (linepoint) {
        if (p2) {
            qDebug() << "line p1 undo";
            static_cast<FlowLine*>(item)->setBeginPoint(QPointF(posX, posY));
        }
        else {
            qDebug() << "line p2 undo";
            static_cast<FlowLine*>(item)->setEndPoint(QPointF(posX, posY));
        }
    }
    else {
        qDebug() << "position undo";
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
            qDebug() << "insert undo";
            scene->removeItem(item);
        }
        else {
            qDebug() << "delete undo";
            scene->addItem(item);
        }
    }
    else {
        qDebug() << "null";
    }
}

RatioChangeCommand::RatioChangeCommand(
        QGraphicsItem *item, QPolygonF polygon) :
    Command{item}, polygon{polygon}
{

}

void RatioChangeCommand::undo() {
    qDebug() << "ratio undo";
    static_cast<FlowPolygon*>(item)->setPolygon(polygon);
}
