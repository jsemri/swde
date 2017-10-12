#ifndef FLOWCHARTITEMS
#define FLOWCHARTITEMS

#include <QMap>
#include <QPixmap>

class FlowItem {
public:
    enum Type {Terminal = 0, Decision, Process, IO, Transition, Place, Arc,
               Line, Arrow, TextField, None};
    static const int flowPolygons = Type::Transition;
    static const int flowRound = Type::Arc;
    static const int flowLines = Type::Arrow;
    static const QMap<Type, QString> tips;

    FlowItem() = default;
    virtual ~FlowItem() = default;
    virtual QPixmap image() const = 0;
};


#endif // FLOWCHARTITEMS

