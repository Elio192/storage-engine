#ifndef STORAGE_ENGINE_H
#define STORAGE_ENGINE_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <utility>
#include <algorithm>
#include "HashTable.h"

class StorageEngine
{
private:
    HashTable m_database {100}; // custom indexing layer hash table
    std::string m_fileName {};  // flat file name for disk persistence
    
    enum class Command          // supported console loop commands
    {
        SET,
        GET,
        DEL,
        STATS,
        SAVE,
        EXIT,
        COMMANDS,
        INVALID
    };
    
    Command parseCommand(const std::string& cmdStr) const; // maps strings to command enums
    
public:
    StorageEngine(const std::string& fileName);            // initializes engine with file path
    void run();                                            // starts main database loop
    
private:
    void loadDatabase();                   // loads memory from file on startup
    void saveDatabase();                   // saves memory back to file on save/exit
    void handleSet(std::stringstream& ss); // processes insertions/updates
    void handleGet(std::stringstream& ss); // retrieves a student record by key
    void handleDel(std::stringstream& ss); // removes a record from the database
    void printStats() const;               // show performance and memory measurements
    void printCommands() const;            // print the list of available comments along with how to format them
};

#endif // !STORAGE_ENGINE_H
