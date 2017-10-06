#ifndef DEBUG_H
#define DEBUG_H

#include <QDebug>

class Debug
{
public:
    enum DebugLevel {none = -1, low = 0, medium = 1, high = 2};
private:
    static DebugLevel level;

private:
    Debug();

public:
    static DebugLevel get_dbg_level() { return level; }
    static void set_dbg_level(DebugLevel lvl) { level = lvl; }

    template<typename T, typename... Args>
    static void debug(DebugLevel level, T first, Args... args);

    template<typename T, typename... Args>
    static void output(T first, Args... args);

    template<typename T>
    static void output(T arg) { qDebug() << arg;}
};

template<typename T, typename... Args>
void Debug::output(T first, Args... args) {
    qDebug() << first;
    output(args...);
}

template<typename T, typename... Args>
void Debug::debug(DebugLevel level, T first, Args... args) {
    if (level != none && level <= get_dbg_level()) {
        output(first, args...);
    }
}

// used when entering to function
#define ENTRY Debug::debug(Debug::high, "Entering:", __func__);

#endif // DEBUG_H
