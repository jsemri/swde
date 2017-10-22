#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>
#include <QDebug>

#include <cassert>
#include <map>

#include "canvas.h"
#include "flowpolygon.h"
#include "aux.h"

FlowPolygon::FlowPolygon
(FlowItem::Type type, QColor color, QPen pen, QGraphicsItem *parent) :
    QGraphicsPolygonItem(parent), FlowItem{}, itemType{type}
{
    polyg = buildPolygon(itemType);
    setPolygon(polyg);
    changeColor(color);
    setPen(pen);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
}


FlowPolygon::FlowPolygon(FlowPolygon *fpolyg) :
    FlowPolygon{fpolyg->itemType, fpolyg->brush().color(),
                fpolyg->pen()}
{
    polyg = fpolyg->polygon();
    setPolygon(polyg);
    setPos(pos());
}

FlowPolygon::FlowPolygon(std::istringstream &data) :
    QGraphicsPolygonItem(0)
{
    qreal x, y, z, xratio, yratio;
    std::string fillColor;
    std::map<std::string, int> vmap = {
        {"pos",0}, {"border", 0}, {"ratio", 0}, {"fill", 0}, {"type", 0}};
    QPen pen;
    int type;

    std::string buf;
    while (getline(data, buf)) {
        qDebug() << buf.c_str();
        std::istringstream ss{buf};
        std::string first; ss >> first;
        if (first == "pos") {
            ss >> x >> y >> z;
        }
        else if (first == "border") {
            int width;
            std::string color;
            ss >> color >> width;
            pen.setColor(QColor(color.c_str()));
            pen.setWidth(width);
        }
        else if (first == "ratio") {
            ss >> xratio >> yratio;
        }
        else if (first == "fill") {
            ss >> fillColor;
        }
        else if (first == "type") {
            ss >> type;
            itemType = FlowItem::Type(type);
        }
        else {
            throw std::runtime_error("Invalid syntax.");
            qDebug() << QString(first.c_str());
        }
        vmap[first]++;
    }

    for (auto &i : vmap) {
        if (i.second != 1) {
            qDebug() << QString(i.first.c_str()) << i.second;
            throw std::runtime_error("Not all polygon sections are competed.");
        }
    }

    polyg = buildPolygon(itemType);
    setPolygon(polyg);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setPen(pen);
    changeColor(QColor(fillColor.c_str()));
    setPos(x,y);
    setZValue(z);
    changeSize(xratio, yratio);
}

QPolygonF FlowPolygon::buildPolygon(FlowItem::Type itemType) {
    QPainterPath path;
    QPolygonF p;
    switch (itemType) {
        case FlowItem::Type::Decision:
            p << QPointF(-50, 0)
              << QPointF(0, 50)
              << QPointF(50, 0)
              << QPointF(0, -50)
              << QPointF(-50, 0);
            break;
        case FlowItem::Type::Process:
            p << QPointF(-40, -40)
              << QPointF(40, -40)
              << QPointF(40, 40)
              << QPointF(-40, 40)
              << QPointF(-40, -40);
            break;
        case FlowItem::Type::Terminal:
            path.addRoundedRect(-40, -30, 80,
                                60, 30, 30);
            p = path.toFillPolygon();
            break;
        case FlowItem::Type::IO:
            p << QPointF(-50, 0)
              << QPointF(-25, 40)
              << QPointF(25, 40)
              << QPointF(50, 0)
              << QPointF(25, -40)
              << QPointF(-25, -40)
              << QPointF(-50, 0);
            break;
        case FlowItem::Type::Circle:
            path.addEllipse(-40, -40, 80, 80);
            p = path.toFillPolygon();
            break;
        default:
            assert(false);
    }

    return p;
}

QPixmap FlowPolygon::image() const
{
    // just output the item image, used for creating buttons
    QPixmap pixmap(150, 150);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 8));
    painter.translate(75, 75);
    painter.drawPolyline(polyg);

    return pixmap;
}

void FlowPolygon::changeColor(QColor color) {
    // change color and set brush gradient
    /*
    this->color = color;
    QLinearGradient gradient(0,0,0,45);
    gradient.setColorAt(0, color);
    gradient.setColorAt(1, Qt::white);
    setBrush(gradient);
    */
    setBrush(color);
}

void FlowPolygon::changeSize(qreal xratio, qreal yratio) {
    if (xratio == 0) xratio = 0.0001;
    if (yratio == 0) yratio = 0.0001;
    QPolygonF newPolyg;
    for (auto & point : polyg) {
        newPolyg.append(QPointF(point.x()*xratio, point.y()*yratio));
    }
    polyg = newPolyg;
    setPolygon(polyg);
}

void FlowPolygon::serialize(std::ofstream &out) const {
    print(out, "FlowPolygon");
    print(out, "type", (int)itemType);
    print(out, "pos", x(), y(), zValue());
    print(out, "border", COLOR2STR(pen().color()), pen().width());
    qreal xratio = polyg.boundingRect().width() /
                   buildPolygon(itemType).boundingRect().width();
    qreal yratio = polyg.boundingRect().height() /
                   buildPolygon(itemType).boundingRect().height();

    print(out, "ratio", xratio, yratio);
    print(out, "fill", COLOR2STR(brush().color()));
}
