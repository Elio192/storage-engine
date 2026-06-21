#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <string>
#include <fstream>

struct StudentRecord
{
    int age;
    std::string major;
    friend std::istream& operator>> (std::istream& in, StudentRecord& sr);
};

std::ostream& operator<< (std::ostream& out, const StudentRecord& sr);

struct ListNode
{
    std::string key {};
    StudentRecord data {};
    ListNode* next {};
    ListNode (const std::string& k, const StudentRecord& d)
    : key {k}, data{d}, next{nullptr}
    {}
};

class HashTable
{
private:
    int m_tableSize;
    ListNode** m_hashTable; // dynamic array of list pointers
    int hashFunction (const std::string& key) const;

public:
    HashTable (int tableSize);
    ~HashTable ();
    
    // inserts a new key-value pair or updates the record if the key exists
    void insert(const std::string& key, const StudentRecord& record);
    
    // looks up a key, returns true if found and copies data to foundRecord
    bool get(const std::string& key, StudentRecord& foundRecord) const;
    
    // removes a key from the table and frees its node memory; returns false if missing
    bool remove(const std::string& key);
    
    // prints diagnostics like total collisions and bucket distribution
    void printStats() const;
    
    // write all active records to the file
    void writeToStream(std::ofstream& oFile);
};

#endif // !HASHTABLE_H
