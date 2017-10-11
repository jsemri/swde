#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QWidget>
#include <QRect>
#include <QDebug>
#include <QTextCursor>

#include "arrow.h"
#include "canvas.h"
#include "textfield.h"

Canvas::Canvas(QMenu *itemMenu, QWidget *parrent) :
    QGraphicsScene{parrent}, itemMenu{itemMenu},
    mode{MoveItem}, itemType{FlowChartItem::None},
    activeItem{nullptr}
{
    setBackgroundBrush(Qt::white);
}

void Canvas::mousePressEvent(QGraphicsSceneMouseEvent *event) {

    FlowChartItem *item;
    TextField *text;
    switch (mode) {
        case InsertItem:
            qDebug() << "inserting item";
            item = new FlowChartItem(itemType, Qt::white, 0.5, itemMenu);
            addItem(item);
            item->setPos(event->scenePos());
            emit itemInserted(item);
            break;
        case MoveItem:
            qDebug() << "moving item";
            activeItem = itemAt(event->scenePos(), QTransform());
            //QGraphicsScene::mousePressEvent(event);
            QGraphicsScene::mousePressEvent(event);
            break;
        case InsertLine:
            startArrowPoint = event->scenePos();
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
        default:
            ;
    };

}

void Canvas::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {

    if (mode == MoveItem) {
        if (activeItem) {
            QGraphicsScene::mouseMoveEvent(event);
        }
    }
}

void Canvas::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {

    if (mode == MoveItem) {
        QGraphicsScene::mouseReleaseEvent(event);
    }
    else if (mode == InsertLine) {
        Arrow *arrow = new Arrow;
        addItem(arrow);
        QLineF line(startArrowPoint, event->scenePos());
        arrow->setLine(line);
        emit arrowInserted();
    }
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
