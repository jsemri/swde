#include <QDebug>

#include "command_manager.h"
#include "commands.h"

CommandManager::CommandManager()
{
    // empty
}

CommandManager::~CommandManager()
{
    while (undoStack.size()) {
        delete undoStack.back();
        undoStack.pop_back();
    }
}

void CommandManager::add(Command *cmd)
{
    undoStack.push_back(cmd);
}

void CommandManager::undo()
{
    undoStack.back()->undo();
    delete undoStack.back();
    undoStack.pop_back();
}
