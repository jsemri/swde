#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QWidget>
#include <QRect>
#include <QDebug>
#include <QTextCursor>
#include "debug.h"
#include "canvas.h"
#include "textfield.h"

Canvas::Canvas(QMenu *item_menu, QWidget *parrent) :
    QGraphicsScene{parrent}, item_menu{item_menu},
    mode{MoveItem}, item_type{FlowChartItem::None}, item_color{Qt::white},
    active_item{nullptr}
{
    setBackgroundBrush(Qt::white);
}

void Canvas::mousePressEvent(QGraphicsSceneMouseEvent *event) {

    ENTRY

    FlowChartItem *item;
    TextField *text;
    switch (mode) {
        case InsertItem:
            qDebug() << "inserting item";
            item = new FlowChartItem(item_type, item_color, 0.5, item_menu);
            addItem(item);
            item->setPos(event->scenePos());
            break;
        case MoveItem:
            qDebug() << "moving item";
            active_item = itemAt(event->scenePos(), QTransform());
            QGraphicsScene::mousePressEvent(event);
            break;
        case InsertText:
            qDebug() << "inserting text";
            text = new TextField();
            text->setZValue(1001);
            //text->setFont(QFont::Normal);
            text->setTextInteractionFlags(Qt::TextEditorInteraction);
            text->setPlainText(QString("asd"));
            addItem(text);
            text->setPos(event->scenePos());
            text->setDefaultTextColor(Qt::black);
            connect(text, SIGNAL(lostFocus(TextField*)), this,
                    SLOT(editorLostFocus(TextField*)));
            connect(text, SIGNAL(selectedChange(QGraphicsItem*)), this,
                    SIGNAL(itemSelected(QGraphicsItem*)));
            emit textInserted(text);
        default:
            ;
    };
    QGraphicsScene::mousePressEvent(event);
}

void Canvas::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {

    ENTRY

    if (mode == MoveItem) {
        if (active_item) {
            QGraphicsScene::mouseMoveEvent(event);
        }
    }
}

void Canvas::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {

    ENTRY

    if (mode == MoveItem) {
        QGraphicsScene::mouseReleaseEvent(event);
        if (active_item) {
            // set item back
            static_cast<FlowChartItem*>(active_item)->set_old_zvalue();
            active_item = nullptr;
        }
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
