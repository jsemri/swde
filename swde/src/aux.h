#ifndef AUX_H
#define AUX_H

#include <QColor>
#include <QString>

// printing serialized data to output (file)

template<typename T1, typename T2>
void print(T1 &out, T2 arg) {
    out << arg << "\n";
}

template<typename T1, typename T2, typename... Args>
void print(T1 &out, T2 arg, Args... args) {
    out << arg << " ";
    print(out, args...);
}

#define COLOR2STR(color) \
    color.name().toUtf8().constData()

#endif // AUX_H

