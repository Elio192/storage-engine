#include "StorageEngine.h"

// map string to command enum
StorageEngine::Command StorageEngine::parseCommand(const std::string& cmdStr) const
{
    if (cmdStr == "SET") return Command::SET;
    else if (cmdStr == "GET") return Command::GET;
    else if (cmdStr == "DEL") return Command::DEL;
    else if (cmdStr == "STATS") return Command::STATS;
    else if (cmdStr == "SAVE") return Command::SAVE;
    else if (cmdStr == "EXIT") return Command::EXIT;
    else if (cmdStr == "COMMANDS") return Command::COMMANDS;
    else return Command::INVALID;
}

// initialize database file name
StorageEngine::StorageEngine(const std::string& fileName)
: m_fileName {fileName}
{ }

// execute main interface loop
void StorageEngine::run()
{
    bool isRunning = true;
    bool isFirstPrompt = true;
    loadDatabase(); // load data from file
    while (isRunning)
    {
        // manage interface formatting
        if (isFirstPrompt)
        {
            std::cout << "StorageEngine> ";
            isFirstPrompt = false;
        }
        else
        {
            std::cout << "\nStorageEngine> ";
        }
        
        std::string line {};
        if (!std::getline(std::cin, line)) break; // stop on stream break
        
        std::stringstream ss(line);
        std::string command {};
        ss >> command; // extract operation
        
        // forward stream tokens to handlers
        switch (parseCommand(command))
        {
            case Command::SET: handleSet(ss); break;
            case Command::GET: handleGet(ss); break;
            case Command::DEL: handleDel(ss); break;
            case Command::STATS: printStats(); break;
            case Command::SAVE:
                saveDatabase();
                std::cout << "SUCCESS: Checkpoint created! Database safely saved to file.\n";
                break;
            case Command::EXIT:
                saveDatabase(); // auto flush on exit
                isRunning = false;
                break;
            case Command::COMMANDS: printCommands(); break;
            default:
                std::cout << "ERROR: Please enter a valid command.\n";
                std::cout << "Enter COMMANDS for the list of available commands.\n";
        }
    }
}

// read data records from file
void StorageEngine::loadDatabase()
{
    std::ifstream inf(m_fileName);
    
    // warn user if file is absent
    if (!inf)
    {
        std::cerr << "ERROR: database.db could not be opened for reading\n";
        std::cerr << "If this is your first time running the program, a file will be created when you exit the program.\n\n";
    }
    else
    {
        std::string tempStr;
        std::string key;
        StudentRecord strData{};
        // parse and populate table line by line
        while (std::getline(inf, tempStr))
        {
            std::stringstream ss {tempStr};
            if (ss >> key && ss >> strData) m_database.insert(key, strData);
        }
    }
}

// save data records to file
void StorageEngine::saveDatabase()
{
    std::ofstream oFile {m_fileName};
    if (!oFile)
    {
        std::cerr << "ERROR: Could not create or write to the database file on exit.\n";
    }
    else
    {
        m_database.writeToStream(oFile); // delegate writing to index layer
    }
}

// insert or modify a database record
void StorageEngine::handleSet(std::stringstream& ss)
{
    std::string key{};
    StudentRecord data{};
    
    // confirm data layout is valid
    if (!(ss >> key && ss >> data))
    {
        std::cout << "ERROR: Invalid syntax. Usage: SET [key] [age] [major].\n";
        std::cout << "For example: SET John 19 CS\n";
        return;
    }
    
    StudentRecord dummy {};
    // override entry if key is active
    if (m_database.get(key, dummy))
    {
        m_database.insert(key, data);
        std::cout << "SUCCESS: Key " << key << " has been updated.\n";
        return;
    }
    m_database.insert(key, data); // store new record
    std::cout << "SUCCESS: Key " << key << " has been stored.\n";
}

// look up a record by key
void StorageEngine::handleGet(std::stringstream& ss)
{
    std::string searchKey{};
    if (!(ss >> searchKey))
    {
        std::cout << "Please enter a key. Usage: GET [key].";
        return;
    }
    
    StudentRecord foundRecord{};
    // inspect index structure
    if (m_database.get(searchKey, foundRecord))
    {
        std::cout << "SUCCESS: Key " << searchKey << " has been found.\n";
        std::cout << searchKey << ' ' << foundRecord << '\n';
    }
    else
    {
        std::cout << "ERROR: Key not found. Please enter a valid key.\n";
    }
}

// remove a record by key
void StorageEngine::handleDel(std::stringstream& ss)
{
    std::string searchKey{};
    if (!(ss >> searchKey))
    {
        std::cout << "Please enter a key. Usage: DEL [key]\n";
        return;
    }
    
    // trigger bucket node tracking logic
    if (m_database.remove(searchKey))
    {
        std::cout << "SUCCESS: Deleted record for " << searchKey << ".\n";
    }
    else
    {
        std::cout << "ERROR: Key not found. Please enter a valid key.\n";
    }
}

// show performance and memory measurements
void StorageEngine::printStats() const
{
    m_database.printStats();
}

// print the list of available comments along with how to format them
void StorageEngine::printCommands() const
{
    std::cout << "\nThe following are the list of available commands in the database:\n\n";
    
    std::cout << "1. SET - add/update to the database.\n";
    std::cout << "\t- Format: SET [key] [age] [major]\n";
    std::cout << "\t- Example: SET John 20 Computer Science\n";
    std::cout << "\t- Note: If a key already exists, the data will be updated instead.\n";
    std::cout << "\t        For duplicate names, create a unique key like John02.\n\n";
    
    std::cout << "2. GET - find the given key and print its data.\n";
    std::cout << "\t- Format: GET [key]\n";
    std::cout << "\t- Example: GET John\n\n";

    std::cout << "3. DEL - find the given key and delete its data.\n";
    std::cout << "\t- Format: DEL [key]\n";
    std::cout << "\t- Example: DEL John\n\n";
    
    std::cout << "4. STATS - display internal database performance metrics and bucket distribution.\n";
    std::cout << "\t- Format: STATS\n\n";

    std::cout << "5. SAVE - explicitly save the current database state to the database file.\n";
    std::cout << "\t- Format: SAVE\n";
    std::cout << "\t- Note: This command is created as an emergency drill in case the program crashes.\n";
    std::cout << "\t        The database will be automatically saved upon exit.\n\n";

    std::cout << "6. EXIT - safely save all pending changes and close the storage engine.\n";
    std::cout << "\t- Format: EXIT\n\n";
    
    std::cout << "7. COMMANDS - print out all the available commands with the format to use them.\n";
    std::cout << "\t- Format: COMMANDS\n\n";
    
    std::cout << "WARNING: The commands are case-sensitive, meaning all letters must be capitalized.\n";
}
