#include "Sql.h"
#include <fstream>
#include <sstream>
#include <string>

std::string Sql::get_query_check_for_task_table()
{
	const char* sql = R"(
		SELECT name
		FROM sqlite_master
		WHERE type = 'table'
		AND name = 'task';
	)";
	return std::string(sql);
}

std::string Sql::get_query_check_for_user_table()
{

	const char* sql = R"(
		SELECT name
		FROM sqlite_master
		WHERE type = 'table'
		AND name = 'user';
	)";
	return std::string(sql);
}

std::string Sql::get_query_create_task_table()
{
	const char* sql = R"(
		CREATE TABLE task (
		        id INTEGER PRIMARY KEY AUTOINCREMENT,
		        name TEXT NOT NULL,
		        description TEXT NOT NULL,
		        priority INTEGER DEFAULT 0,
		        userid INTEGER NOT NULL,
		        created_at DATETIME DEFAULT CURRENT_TIMESTAMP
		);
	)";
	return std::string(sql);
}

std::string Sql::get_query_create_user_table()
{
	const char* sql = R"(
		CREATE TABLE user (
			id INTEGER PRIMARY KEY AUTOINCREMENT,
			username TEXT UNIQUE NOT NULL,
			password TEXT NOT NULL
		);
	)";
	return std::string(sql);
}

std::string Sql::get_query_fetch_user()
{
	const char* sql = R"(
		SELECT * FROM user AS u -- Replace '*' with necessary columns.
		WHERE u.username = ?;
	)";
	return std::string(sql);
}

std::string Sql::get_query_insert_task()
{
	const char* sql = R"(
		INSERT INTO task (name, description, userid)
		VALUES (?, ?, ?);
	)";
	return std::string(sql);
}

std::string Sql::get_query_insert_user()
{
	const char* sql = R"(
		INSERT INTO user (username, password)
		VALUES (?, ?);
	)";
	return std::string(sql);
}

std::string Sql::get_query_select_all_tasks()
{
	const char* sql = R"(
		SELECT * FROM task AS t -- Replace '*' with necessary columns.
		WHERE t.userid = ?;
	)";
	return std::string(sql);
}

std::string Sql::get_query_select_task()
{
	const char* sql = R"(
		SELECT * FROM task AS t
		WHERE t.id = ?;
	)";
	return std::string(sql);
}

std::string Sql::get_query_select_user()
{
	const char* sql = R"(
		SELECT * FROM user -- Replace '*' with necessary columns.
		WHERE username = ?
		AND password = ?;
	)";
	return std::string(sql);
}
