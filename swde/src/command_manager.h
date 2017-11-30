#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include <vector>

#include "commands.h"

class Command;

class CommandManager
{
private:
    std::vector<Command*> undoStack;

public:
    CommandManager();
    ~CommandManager();

    void add(Command *cmd);
    void undo();
    bool isEmpty() const { return undoStack.empty();}
    void clear();
};

#endif // COMMANDMANAGER_H
