#include "flowitem.h"

const QMap<FlowItem::Type, QString> FlowItem::tips{
        {FlowItem::Type::Terminal, QString("Terminal")},
        {FlowItem::Type::Decision, QString("Decision")},
        {FlowItem::Type::Process, QString("Process")},
        {FlowItem::Type::IO, QString("Input/Output")},
        {FlowItem::Type::Transition, QString("Transition")},
        {FlowItem::Type::Place, QString("Place - PT")},
        {FlowItem::Type::Arc, QString("Process - PT")},
        {FlowItem::Type::Line, QString("Line")},
        {FlowItem::Type::Arrow, QString("Arrow")},
        {FlowItem::Type::TextField, QString("Text field")}
    };
