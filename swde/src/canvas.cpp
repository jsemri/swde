#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QWidget>
#include <QRect>
#include <QDebug>
#include <QTextCursor>

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
            QGraphicsScene::mousePressEvent(event);
            break;
        case InsertLine:
            qDebug() << "inserting line";
            arrow = new FlowLine(event->scenePos(),event->scenePos());
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

    if (mode == MoveItem) {
        if (activeItem) {
            QGraphicsScene::mouseMoveEvent(event);
        }
    }
    else if (mode == InsertLine && arrow) {
        arrow->setEndPoint(event->scenePos());
        arrow->update();
    }
}

void Canvas::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {

    if (mode == MoveItem) {
        QGraphicsScene::mouseReleaseEvent(event);
    }
    else if (mode == InsertLine) {
        //arrow->setEndPoint(event->scenePos());
        arrow = 0;
        emit arrowInserted();
        QGraphicsScene::mouseReleaseEvent(event);
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
