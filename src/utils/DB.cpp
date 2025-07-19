#include "./DB.h"
#include <string>
#include "../core/Task.h"
#include "../core/User.h"
#include "../utils/Crypto.h"
#include "../utils/QueryParam.h"
#include "../utils/QueryStmt.h"
#include "../utils/Sql.h"
#include "../../external/sqlite/sqlite3.h"

bool DB::exists_table(sqlite3* db, std::string exists_table)
{
	sqlite3_stmt* stmt{nullptr};
	if (sqlite3_prepare_v2(db, exists_table.c_str(), -1, &stmt, 0) != SQLITE_OK) {
		return false;
	}
	return (sqlite3_step(stmt) == SQLITE_ROW) ? true : false;
}

bool DB::create_table(sqlite3* db, std::string create_table)
{
	sqlite3_stmt* stmt{nullptr};
	if (sqlite3_prepare_v2(db, create_table.c_str(), -1, &stmt, 0) != SQLITE_OK) {
		return false;
	}
	return (sqlite3_step(stmt) == SQLITE_DONE) ? true : false;
}

std::vector<Task> DB::get_tasks(sqlite3* db, User& user)
{
    /* Fetch tasks from DB. */
    QueryStmt select_tasks = QueryStmt::create()
        .set_sql_query(Sql::select_all_tasks())
        .prepare(db)
        .add_param(1, QueryParam(user.id))
        .build();

    /* Create vector of tasks. */
    std::vector<Task> all_tasks;
    while (sqlite3_step(select_tasks.stmt_) == SQLITE_ROW) {
        Task task = Task::create()
            .set("id", select_tasks.stmt_, 0)
            .set("name", select_tasks.stmt_, 1)
            .set("description", select_tasks.stmt_, 2)
            .set("priority", select_tasks.stmt_, 3)
            .set("user_id", select_tasks.stmt_, 4)
            .set("created_at", select_tasks.stmt_, 5)
            .build();
        all_tasks.push_back(task);
    };
    return all_tasks;
}

void DB::assign_user_data(QueryStmt& stmt, User& user)
{
    unsigned int user_id{(unsigned int) sqlite3_column_int(stmt.stmt_, 0)};
    std::string username_b64{(const char*) sqlite3_column_text(stmt.stmt_, 1)};
    std::string username_bin{Crypto::to_bin(username_b64)};
    std::string username_decrypted{Crypto::decrypt(username_bin)};
    user.id = user_id;
    user.username = username_decrypted;
}
