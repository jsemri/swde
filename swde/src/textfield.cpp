#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QDebug>

#include "aux.h"
#include "textfield.h"
#include "canvas.h"

TextField::TextField(QGraphicsItem *parent) :
    QGraphicsTextItem{parent}
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
}

TextField::TextField(TextField *textField) :
    TextField{}
{
    setFont(textField->font());
    setPlainText(textField->toPlainText());
    setPos(textField->pos());
}

QVariant
TextField::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == QGraphicsItem::ItemSelectedHasChanged) {
        emit selectedChange(this);
    }
    return value;
}

void TextField::focusOutEvent(QFocusEvent *event) {
    setTextInteractionFlags(Qt::NoTextInteraction);
    emit lostFocus(this);
    QGraphicsItem::focusOutEvent(event);
}

void TextField::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    if (textInteractionFlags() == Qt::NoTextInteraction) {
        setTextInteractionFlags(Qt::TextEditorInteraction);
    }
    QGraphicsTextItem::mouseDoubleClickEvent(event);
}

QPixmap TextField::image() const {
    QPixmap pixmap(250, 250);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 8));
    QFont myFont;
    myFont.setPixelSize(100);
    painter.setFont(myFont);
    painter.drawText(20, 150, QString("Text"));

    return pixmap;
}

TextField::TextField(std::istringstream &data) {
    qreal x, y, z;
    QFont font;
    std::map<std::string, int> vmap = {
        {"pos",0}, {"font", 0}, {"text", 0}};

    std::string buf;
    while (getline(data, buf)) {
        std::istringstream ss{buf};
        std::string first; ss >> first;
        if (first == "pos") {
            ss >> x >> y >> z;
        }
        else if (first == "text") {
            std::string str;
            ss >> str;
            std::string text;
            for (size_t i = 0; i < str.length(); i+=2) {
                std::string s = str.substr(i,2);
                char byte = (char)(int)strtol(s.c_str(), 0, 16);
                text.push_back(byte);
            }
            setPlainText(QString(text.c_str()));
        }
        else if (first == "font") {
            int size, bold, italic, underline;
            ss >> size >> bold >> italic >> underline;
            font.setPixelSize(size);
            font.setBold(bold);
            font.setItalic(italic);
            font.setUnderline(underline);
            setFont(font);
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

    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setPos(x,y);
    setZValue(z);
}

void TextField::serialize(std::ofstream &out) const {
    print(out, "TextField");
    print(out, "pos", x(), y(), zValue());
    std::string text = toPlainText().toUtf8().constData();
    out << "text ";
    for (int i = 0; i < text.length(); i++) {
        out << std::hex << int(text[i]);
    }
    out << "\n";
    qDebug() << font().pointSize();
    int size = font().pointSize();
    print(out, "font", std::to_string(size), font().bold(), font().italic(),
          font().underline());
}
