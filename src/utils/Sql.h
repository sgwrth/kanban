#ifndef SQL_H
#define SQL_H
#include <string>

class Sql {
public:
	static std::string check_for_task_table();
	static std::string check_for_user_table();
	static std::string create_task_table();
	static std::string create_user_table();
	static std::string fetch_user();
	static std::string insert_task();
	static std::string insert_user();
	static std::string select_all_tasks();
	static std::string select_task();
	static std::string select_user();
};

#endif
