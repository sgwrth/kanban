#include <iostream>
#include <memory>
#include <string>
#include <unistd.h>
#include <vector>
#include "../app/Menu_item.h"
#include "../core/Task.h"
#include "../utils/Encrypt.h"
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

	/* check if user table exists */
	const char* check_for_user_table =
			"SELECT name "
			"FROM sqlite_master "
			"WHERE type = 'table' "
			"AND name NOT LIKE 'sqlite_%';";
	sqlite3_stmt* stmt;
	rc = sqlite3_prepare_v2(db, check_for_user_table, -1, &stmt, 0);
	std::cout << "rc prepare: " << rc << "\n";
	rc = sqlite3_step(stmt);
	if (rc != SQLITE_ROW) {
		/* no user table found -> create it */
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
	
	// user menu
	std::cout << "rc (user table present): " << rc << "\n";
	std::vector<std::string> user_menu_options;
	user_menu_options.push_back("Log in");	
	user_menu_options.push_back("Create user");	
	std::vector<std::unique_ptr<Menu_item>> user_menu;
	for (int i = 0; i < user_menu_options.size(); ++i) {
		user_menu.push_back(std::make_unique<Menu_item>(i, user_menu_options[i]));
	}
	// (extract:)
	std::string user_choice {};
	do {
		std::cout << "User menu:\n";
		for (const auto& option : user_menu) {
			std::cout << "[" << option->get_number() << "] "
					<< option->get_name() + "\n";
		}
		std::cout << "Choice: \n";
		std::cin.clear();
		std::cin >> user_choice;
	} while (!Input::is_valid_menu_option(user_choice, user_menu));

	if (user_choice == "0") {

		/*
		 * Enter credentials.  Also, extract this.
		 */
		std::cout << "Enter username\n";
		std::string username {};
		std::cin >> username;
		char* pw = getpass("Enter password: ");
		std::string password {pw};
		std::string password_encrypted {Encrypt::sha256(password)};

		/*
		 * Fetching user from DB.  If user exists, get the 'username' column.
		 */
		std::string select_user =
				"SELECT * FROM user "
				"WHERE username = ? "
				"AND password = ?;";
		sqlite3_stmt* select_user_stmt;
		rc = sqlite3_prepare_v2(db, select_user.c_str(), -1, &select_user_stmt, 0);
		std::cout << "rc prepare select user: " << rc << "\n";
		rc = sqlite3_bind_text(select_user_stmt, 1, username.c_str(), -1, SQLITE_STATIC);
		std::cout << "bind select user, username: " << rc << "\n";
		rc = sqlite3_bind_text(select_user_stmt, 2, password_encrypted.c_str(), -1, SQLITE_STATIC);
		std::cout << "bind select user, password: " << rc << "\n";
		rc = sqlite3_step(select_user_stmt);
		std::cout << "rc select user table: " << rc << "\n";

		if (rc != SQLITE_ROW) {
			std::cout << "Login failed.  Good bye.\n";
			return -1;
		}

		std::string username_from_db = (const char*) sqlite3_column_text(select_user_stmt, 1);
		std::cout << "Login successful!  Welcome, " << username_from_db << "\n";
	}

	if (user_choice == "1") {

		/*
		 * Enter credentials.  Also, extract this.
		 */
		std::cout << "Enter username:\n";
		std::string username {};
		std::cin >> username;
		char* pw = getpass("Enter password: ");
		std::string password {pw};
		std::string password_encrypted {Encrypt::sha256(password)};

		sqlite3_stmt* insert_user_stmt;
		const char* insert_user =
				"INSERT INTO user (username, password) "
				"VALUES (?, ?);";
		rc = sqlite3_prepare_v2(db, insert_user, -1, &insert_user_stmt, 0);
		std::cout << "rc prepare insert user: " << rc << "\n";
		rc = sqlite3_bind_text(insert_user_stmt, 1, username.c_str(), -1, SQLITE_STATIC);
		std::cout << "bind insert user, username: " << rc << "\n";
		rc = sqlite3_bind_text(insert_user_stmt, 2, password_encrypted.c_str(), -1, SQLITE_STATIC);
		std::cout << "bind insert user, password: " << rc << "\n";
		rc = sqlite3_step(insert_user_stmt);
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

	// (extract:)
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
