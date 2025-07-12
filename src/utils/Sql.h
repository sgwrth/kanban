#ifndef SQL_H
#define SQL_H
#include <string>

class Sql {
public:
	static std::string get_query_check_for_task_table();
	static std::string get_query_check_for_user_table();
	static std::string get_query_create_task_table();
	static std::string get_query_create_user_table();
	static std::string get_query_fetch_user();
	static std::string get_query_insert_task();
	static std::string get_query_insert_user();
	static std::string get_query_select_all_tasks();
	static std::string get_query_select_task();
	static std::string get_query_select_user();
};

#endif
