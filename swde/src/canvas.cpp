#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QWidget>
#include <QRect>
#include <QDebug>
#include <QTextCursor>
#include <math.h>

#include "flowline.h"
#include "canvas.h"
#include "textfield.h"

Canvas::Canvas(QMenu *itemMenu, QWidget *parrent) :
    QGraphicsScene{parrent}, itemMenu{itemMenu},
    mode{MoveItem}, itemType{FlowItem::Type::None},
    activeItem{nullptr}
{
    setBackgroundBrush(Qt::white);
}

void Canvas::mousePressEvent(QGraphicsSceneMouseEvent *event) {

    FlowPolygon *item;
    TextField *text;
    switch (mode) {
        case InsertItem:
            qDebug() << "inserting item";
            item = new FlowPolygon(itemType, Qt::white, 0.5, itemMenu);
            addItem(item);
            item->setPos(event->scenePos());
            emit itemInserted(item);
            break;
        case MoveItem:
            qDebug() << "moving item";
            activeItem = itemAt(event->scenePos(), QTransform());
            if (activeItem &&
                activeItem->type() == FlowLine::Type)
            {
                arrow = static_cast<FlowLine*>(activeItem);
                qreal x = event->scenePos().x();
                if (fabs(arrow->line().p1().x() + arrow->x() - x) < 10)
                {
                    mode = MoveLineP1;
                }
                else if (fabs(arrow->line().p2().x() + arrow->x() - x) < 10)
                {
                    mode = MoveLineP2;
                }
            }
            QGraphicsScene::mousePressEvent(event);
            break;
        case InsertLine:
            qDebug() << "inserting line";
            arrow = new FlowLine(itemType == FlowItem::Type::Arrow,
                                 event->scenePos(),event->scenePos());
            addItem(arrow);
            break;
        case InsertText:
            qDebug() << "inserting text";
            text = new TextField();
            text->setZValue(1001);
            //text->setFont(QFont::Normal);
            text->setTextInteractionFlags(Qt::TextEditorInteraction);
            addItem(text);
            text->setPos(event->scenePos());
            text->setDefaultTextColor(Qt::black);
            connect(text, SIGNAL(lostFocus(TextField*)), this,
                    SLOT(editorLostFocus(TextField*)));
            connect(text, SIGNAL(selectedChange(QGraphicsItem*)), this,
                    SIGNAL(itemSelected(QGraphicsItem*)));
            emit textInserted(text);
            QGraphicsScene::mousePressEvent(event);
            break;
        default:
            qDebug() << "undefined operation";
    };

}

void Canvas::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    switch (mode) {
        case MoveItem:
            if (activeItem) {
                QGraphicsScene::mouseMoveEvent(event);
            }
            break;
        case InsertLine:
            // arrow has to be set before it is modified
            if (!arrow) {
                break;
            }
        case MoveLineP2:
            arrow->setEndPoint(event->scenePos());
            arrow->update();
            break;
        case MoveLineP1:
            arrow->setBeginPoint(event->scenePos());
            arrow->update();
        default:
            ;
            //QGraphicsScene::mouseMoveEvent(event);
    }
}

void Canvas::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {

    switch (mode) {
        case MoveLineP2:
        case MoveLineP1:
            mode = MoveItem;
        case InsertLine:
            arrow = 0;
        case MoveItem:
            QGraphicsScene::mouseReleaseEvent(event);
        default:
            ;
    }
    //QGraphicsScene::mouseReleaseEvent(event);
}

void Canvas::editorLostFocus(TextField *item) {
    qDebug() << "lost focus";
    QTextCursor cursor = item->textCursor();
    cursor.clearSelection();
    item->setTextCursor(cursor);
    if (item->toPlainText().isEmpty()) {
        removeItem(item);
        item->deleteLater();
    }
}
