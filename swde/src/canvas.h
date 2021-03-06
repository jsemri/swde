#ifndef CANVAS_H
#define CANVAS_H

#include <QGraphicsScene>
#include <QWidget>
#include <QList>
#include <QFont>

#include "flowpolygon.h"
#include "flowitem.h"
#include "command_manager.h"

class Qmenu;
class TextField;
class QGraphicsTextItem;
class FlowLine;

// Main class for managing graphics items
class Canvas : public QGraphicsScene
{
    Q_OBJECT
public:
    // states of canvas
    enum Mode {InsertItem, InsertLine, InsertText, MoveItem, MoveLineP1,
               MoveLineP2, ResizeItem, Idle};
private:
    // has been canvas modifyied flag
    bool modified;
    Mode mode;
    // type of inserted item
    FlowItem::Type itemType;
    // auxiliary variables for item manipulation
    QGraphicsItem *activeItem;
    QPointF startArrowPoint;
    bool startArrowPointSet;
    FlowLine *arrow;
    // variables for item modification
    QColor itemColor;
    QPen itemPen;
    QFont textFont;
    // z coordinate of newly added item
    qreal ZValue;
    // undo history
    CommandManager commands;

    void getInside(QGraphicsItem *item) const;
    QPointF getInside(QPointF point) const;
    bool isInside(QPointF point) const;
    qreal getZValue() { ZValue += 0.01; return ZValue;}
    QGraphicsItem *
    loadItem(const std::string &itemStr, const std::string &data);
    QGraphicsItem* copyItem(QGraphicsItem *itemCopy);
    void addToHistory(Command *cmd);

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

public:
    explicit Canvas(QWidget *parrent=0);
    void pasteItem(QGraphicsItem *itemCopy);
    void resize(int width, int h);
    void setFont(const QFont &font);
    void setItemColor(QColor color) { itemColor = color; }
    QPen pen() const { return itemPen; }
    void save(const QString &file);
    void load(const QString &file);
    bool isModified() const { return modified; }
    void setModified(bool m) { modified = m; }
    void undo();
    void remove();
    void clearHistory();

public slots:
    void setMode(Mode m = MoveItem) { mode = m; }
    void setItemType(FlowItem::Type type) { itemType = type; }
    void editorLostFocus(TextField *item);
    void penWidthChanged(int width);
    void penColorChanged(QColor color);
    void borderButtonClicked();    

signals:
    void textInserted(QGraphicsTextItem *item);
};

#endif // CANVAS_H
