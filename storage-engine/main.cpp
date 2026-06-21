#include "StorageEngine.h"

int main()
{
    StorageEngine database {"database.db"}; // instantiate a database
    database.run();
    return 0;
}
