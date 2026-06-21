#include "HashTable.h"

// read student record from input stream
std::istream& operator>> (std::istream& in, StudentRecord& sr)
{
    in >> sr.age;
    std::getline(in >> std::ws, sr.major); // consume whitespace and read string
    std::size_t temp = sr.major.find_last_not_of(" \t\n\r\f\v");
    if (temp == std::string::npos)
    {
        sr.major.clear(); // empty string if only trailing whitespace
    }
    else
    {
        sr.major.erase(temp + 1, sr.major.size()); // trim trailing whitespace
    }
    return in;
}

// output student record to stream
std::ostream& operator<< (std::ostream& out, const StudentRecord& sr)
{
    out << sr.age << ' ' << sr.major;
    return out;
}

// sum characters to compute bucket index
int HashTable::hashFunction(const std::string& key) const
{
    int sum {0};
    for (const char& c : key)
    {
        sum += c;
    }
    return sum % m_tableSize;
}

// allocate bucket array and clear elements
HashTable::HashTable(int tableSize)
: m_hashTable{}, m_tableSize {tableSize}
{
    m_hashTable = new ListNode*[tableSize];
    for (int i {0}; i < m_tableSize; ++i)
    {
        m_hashTable[i] = nullptr;
    }
}

// free all dynamic memory allocations
HashTable::~HashTable()
{
    if (!m_hashTable) return;
    for (int i{0}; i < m_tableSize; ++i)
    {
        ListNode* curr = m_hashTable[i];
        while (curr != nullptr) // traverse bucket chains
        {
            ListNode* next = curr->next;
            delete curr;
            curr = next;
        }
    }
    delete []m_hashTable; // free bucket pointer array
    m_hashTable = nullptr;
    m_tableSize = 0;
}

// add new item or overwrite existing value
void HashTable::insert(const std::string& key, const StudentRecord& record)
{
    int index {hashFunction(key)};
    ListNode* curr = m_hashTable[index];
    while (curr != nullptr) // check for existing key
    {
        if (curr->key == key)
        {
            curr->data = record; // replace existing record
            return;
        }
        curr = curr->next;
    }
    ListNode* node = new ListNode {key, record}; // create entry if not found
    node->next = m_hashTable[index]; // point to old head
    m_hashTable[index] = node; // set new head
}

// retrieve a record by key matching
bool HashTable::get(const std::string& key, StudentRecord& foundRecord) const
{
    int index {hashFunction(key)};
    const ListNode* curr = m_hashTable[index];
    
    while (curr != nullptr) // traverse bucket chain
    {
        if (curr->key == key)
        {
            foundRecord = curr->data; // copy back data matching key
            return true;
        }
        curr = curr->next;
    }
    return false; // key not found
}

// delete node matching key from storage
bool HashTable::remove(const std::string& key)
{
    int index {hashFunction(key)};
    ListNode* curr = m_hashTable[index];
    
    if (curr != nullptr && curr->key == key) // handle head node matches
    {
        ListNode* temp = m_hashTable[index]->next;
        delete m_hashTable[index];
        m_hashTable[index] = temp;
        return true;
    }
    
    ListNode* prev = curr;
    if (curr != nullptr) curr = curr->next;
    
    while (curr != nullptr) // traverse deeper down chain
    {
        if (curr->key == key)
        {
            prev->next = curr->next; // bridge around target node
            delete curr;
            return true;
        }
        prev = curr;
        curr = curr->next;
    }
    return false; // element not found
}

// display table diagnostic metadata
void HashTable::printStats() const
{
    int totalItems {};
    int usedBuckets {};
    int totalCollisions {};
    
    for (int i{0}; i < m_tableSize; ++i)
    {
        const ListNode* curr = m_hashTable[i];
        if (curr == nullptr) continue;
        
        int chainLength {0};
        while (curr != nullptr) // count entries in active bucket
        {
            ++totalItems;
            ++chainLength;
            curr = curr->next;
        }
        if (chainLength > 1) totalCollisions += chainLength - 1; // collisions are extra items
        ++usedBuckets;
    }
    std::cout << "DATABASE HEALTH METRICS:\n";
    std::cout << "Bucket Utilization: " << usedBuckets << " used out of " << m_tableSize << " slots\n";
    std::cout << "Total Items Stored: " << totalItems << '\n';
    std::cout << "Total Hash Collisions: " << totalCollisions << '\n';
}

// sort entries and write out to file
void HashTable::writeToStream(std::ofstream& oFile)
{
    std::vector<std::pair<std::string, StudentRecord>> tempEntries{};
    for (int i {0}; i < m_tableSize; ++i) // collect all nodes
    {
        ListNode* curr = m_hashTable[i];
        while (curr != nullptr)
        {
            tempEntries.push_back({curr->key, curr->data});
            curr = curr->next;
        }
    }
    
    std::sort(tempEntries.begin(), tempEntries.end(), [] (const auto& pair1, const auto& pair2)
    {
        return pair1.first < pair2.first; // alphabetical sort on key
    });
    
    for (auto const& temp : tempEntries) // write sorted entries out to file
    {
        oFile << temp.first << ' ' << temp.second << '\n';
    }
}
