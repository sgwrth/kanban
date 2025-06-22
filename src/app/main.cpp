#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "../app/Menu_item.h"
#include "../core/Task.h"
#include "../utils/Input.h"
#include "../../external/sqlite/sqlite3.h"

int main()
{
	sqlite3* db;
	int rc = sqlite3_open("data.db", &db);
	if (rc != SQLITE_OK) {
		std::cerr << "Could not open DB: " << sqlite3_errmsg(db) << "\n";
		return rc;
	}

	const char* check_for_user_table =
			"SELECT name "
			"FROM sqlite_master "
			"WHERE type = 'table' "
			"AND name NOT LIKE 'sqlite_%';";
	sqlite3_stmt* stmt;
	rc = sqlite3_prepare_v2(db, check_for_user_table, -1, &stmt, 0);
	std::cout << "rc prepare: " << rc << "\n";
	rc = sqlite3_step(stmt);
	if (rc != SQLITE_ROW) { /* no table 'user' found */
		std::cout << "rc step: " << rc << "\n";
		const char* create_user_table =
				"CREATE TABLE user ("
				"id INTEGER PRIMARY KEY AUTOINCREMENT,"
				"username TEXT UNIQUE NOT NULL,"
				"password TEXT NOT NULL);";
		rc = sqlite3_prepare_v2(db, create_user_table, -1, &stmt, 0);
		std::cout << "rc prepare create user table: " << rc << "\n";
		rc = sqlite3_step(stmt);
		std::cout << "rc create user table: " << rc << "\n";
	}

	std::vector<std::string> menu_options;
	menu_options.push_back("Create a new task");
	menu_options.push_back("Show all tasks");
	menu_options.push_back("Delete a task");

	std::vector<std::unique_ptr<Menu_item>> menu;
	for (int i = 0; i < menu_options.size(); ++i) {
		menu.push_back(std::make_unique<Menu_item>(i, menu_options[i]));
	}

	std::string choice {};
	do {
		std::cout << "Menu:\n";
		for (const auto& option : menu) {
			std::cout << "[" << option->get_number() << "] "
					<< option->get_name() + "\n";
		}
		std::cout << "Choice: \n";
		std::cin.clear();
		std::cin >> choice;
	} while (!Input::is_valid_menu_option(choice, menu));

	sqlite3_close(db);
}
