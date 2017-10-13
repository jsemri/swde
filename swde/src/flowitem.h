#ifndef FLOWCHARTITEMS
#define FLOWCHARTITEMS

#include <QMap>
#include <QPixmap>

class FlowItem {
public:
    enum Type {Terminal = 0, Decision, Process, IO, Circle,
               Line, Arrow, TextField, None};
    static const int flowNodes = Type::Circle;
    static const int flowLines = Type::Arrow;
    static const QMap<Type, QString> tips;

    FlowItem() = default;
    virtual ~FlowItem() = default;
    virtual QPixmap image() const = 0;
};


#endif // FLOWCHARTITEMS

