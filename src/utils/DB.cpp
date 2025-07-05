#include "./DB.h"
#include <string>
#include "../../external/sqlite/sqlite3.h"

bool DB::exists_table(sqlite3* db, std::string check_for_table_query)
{
	sqlite3_stmt* stmt{nullptr};
	if (sqlite3_prepare_v2(db, check_for_table_query.c_str(), -1, &stmt, 0) != SQLITE_OK) {
		return false;
	}
	return (sqlite3_step(stmt) == SQLITE_ROW) ? true : false;
}

bool DB::create_table(sqlite3* db, std::string create_table_query)
{
	sqlite3_stmt* stmt{nullptr};
	if (sqlite3_prepare_v2(db, create_table_query.c_str(), -1, &stmt, 0) != SQLITE_OK) {
		return false;
	}
	return (sqlite3_step(stmt) == SQLITE_ROW) ? true : false;
}
