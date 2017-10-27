#include <QPainter>
#include <QPen>
#include <QDebug>

#include <math.h>
#include "flowline.h"
#include "aux.h"

const qreal Pi = 3.14;

FlowLine::FlowLine(bool arrowSet, QPen pen,
        QPointF beginPoint, QPointF endPoint,
        QGraphicsItem *parent) :
    QGraphicsLineItem(parent), arrowSet{arrowSet}
{
    setLine(QLineF(beginPoint, endPoint));
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setPen(pen);
}

FlowLine::FlowLine(FlowLine *fline) :
    FlowLine{fline->arrowSet, fline->pen(), fline->line().p1(),
             fline->line().p2()}
{
    setPos(fline->pos());
}

QRectF FlowLine::boundingRect() const {
    qreal extra = (pen().width() + 10) / 2.0;
    return QRectF(line().p1(), line().p2()).normalized()
           .adjusted(-extra, -extra, extra, extra);
}

QPainterPath FlowLine::shape() const {
    QPainterPath path = QGraphicsLineItem::shape();
    if (arrowSet) {
        path.addPolygon(arrowHead);
    }
    return path;
}

void FlowLine::paint(
    QPainter *painter, const QStyleOptionGraphicsItem *option,
    QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    if (fabs(line().dx()) < 5 && fabs(line().dy()) < 5) {
        return;
    }

    painter->setPen(pen());
    painter->setBrush(pen().color());
    if (arrowSet) {
        double alpha = ::acos(line().dx() / (line().length()+0.00001));
        if (line().dy() <= 0) {
            alpha = (Pi * 2) - alpha;
        }
        double beta = Pi/12;
        int size = 15 + pen().width()*2;
        QPointF p1, p2;
        p1 = line().p2() - QPointF(cos(alpha - beta) * size,
                                 sin(alpha - beta) * size);
        p2 = line().p2() - QPointF(sin(Pi/2 - alpha - beta) * size,
                                 cos(Pi/2 - alpha - beta) * size);

        arrowHead.clear();
        arrowHead << line().p2() << p1 << p2;
        painter->drawPolygon(arrowHead);
    }

    painter->drawLine(line());
}

QPixmap FlowLine::image() const {
    QPixmap pixmap(250, 250);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 8));
    if (arrowSet) {
        QPolygonF arrow;
        arrow << QPointF(240, 10) << QPointF(240, 50) << QPointF(200, 10);
        painter.drawPolygon(arrow);
    }
    painter.drawLine(10, 240, 240, 10);

    return pixmap;
}

void FlowLine::serialize(std::ofstream &out) const {
    print(out, "FlowLine");
    print(out, "pos", x(), y(), zValue());
    print(out, "border", COLOR2STR(pen().color()), pen().width());
    print(out, "hasarrow", (int)arrowSet);
    print(out, "points", line().p1().x(), line().p1().y(),
          line().p2().x(), line().p2().y());
}

FlowLine::FlowLine(std::istringstream &data) :
    QGraphicsLineItem(0)
{
    qreal x, y, z, x1, x2, y1, y2;
    std::map<std::string, int> vmap = {
        {"pos",0}, {"border", 0}, {"hasarrow", 0}, {"points", 0}};
    QPen pen;
    int arrow = 0;

    std::string buf;
    while (getline(data, buf)) {
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
        else if (first == "points") {
            ss >> x1 >> y1 >> x2 >> y2;
        }
        else if (first == "hasarrow") {
            ss >> arrow;
        }
        else {
            throw std::runtime_error("Invalid FlowLine syntax.");
            qDebug() << QString(first.c_str());
        }
        vmap[first]++;
    }

    for (auto &i : vmap) {
        if (i.second != 1) {
            qDebug() << QString(i.first.c_str()) << i.second;
            throw std::runtime_error("Not all line sections are competed.");
        }
    }

    setLine(QLineF(x1, y1, x2, y2));
    arrowSet = arrow;
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setPen(pen);
    setPos(x,y);
    setZValue(z);
}

