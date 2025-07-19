#ifndef DB_H
#define DB_H
#include <string>
#include <vector>
#include "../core/Task.h"
#include "../core/User.h"
#include "../utils/QueryStmt.h"
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
    static std::vector<Task> get_tasks(sqlite3* db, User& user);
    static void assign_user_data(QueryStmt& stmt, User& user);
};

#endif
