#ifndef DB_H
#define DB_H
#include <string>
#include "../../external/sqlite/sqlite3.h"

class DB {
public:
	static bool exists_table(
        sqlite3* db,
        const std::string check_for_table_query
    );
	static bool create_table(
        sqlite3* db,
        const std::string create_table_query
    );
};

#endif
