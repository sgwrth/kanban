#include <iostream>
#include <libgen.h> /* Needed for dirname function. */
#include <memory>
#include <string>
#include <type_traits>
#include <unistd.h>
#include <vector>
#include "../app/Menu_item.h"
#include "../core/Task.h"
#include "../core/User.h"
#include "../utils/Crypto.h"
#include "../utils/DB.h"
#include "../utils/Input.h"
#include "../utils/Menu.h"
#include "../utils/QueryStmt.h"
#include "../utils/Sql.h"
#include "../utils/Text.h"
#include "../../external/sqlite/sqlite3.h"

int main()
{
	/*
	 * Non-portable, Linux-specific way of getting the directory
	 * in which the binary is located.  This ensures that the DB
	 * will be created in that same directory, not the directory
	 * from which the binary is called for execution.
	 */
	auto buffer = std::vector<char>(1024);
	const ssize_t len = readlink("/proc/self/exe", buffer.data(), buffer.size() - 1);
	buffer[len] = '\0'; /* Terminate the path. */
	const std::string binary_path{buffer.data()}; /* Create a copy. */
	const std::string binary_dir{dirname(buffer.data())}; /* 'dirname' modifies buffer.data() in place. */

	/* Prepare filepath for DB creation. */
	constexpr const char* db_filename = "data.db";
	const std::string db_fullpath = binary_dir + '/' + db_filename;

	/* Open (or, if it doesn't exist) create DB. */
	sqlite3* db{nullptr};
	int rc = sqlite3_open(db_fullpath.c_str(), &db);
	if (rc != SQLITE_OK) {
		std::cerr << "Could not open DB: " << sqlite3_errmsg(db) << '\n';
		return rc;
	}

	/* Check if user table exists.  If not, create it. */
	if (!DB::exists_table(db, Sql::get_query_check_for_user_table())) {
		if (!DB::create_table(db, Sql::get_query_create_user_table())) {
			std::cout << "Error: creating user table failed.\n";
			return 1;
		}
	}
	
	/* (Replace with Builder pattern.) */
	auto user_menu = Menu::create("Log in", "Create user");

	User logged_in_user;

	/* Show user menu and get user's choice. */
	std::string user_choice = Input::get_menu_option_choice(user_menu, "User menu"); 

	/* Log in. */
	if (user_choice == "0") {

		/* Enter credentials.  (Also, extract this.) */
		std::cout << "Enter username: ";
		std::string username{};
		std::getline(std::cin, username);
		std::string pw_cstring = getpass("Enter password: ");
		std::string password{pw_cstring};
		std::string pw_hashed{Crypto::sha256(password)};

		/* Encrypt username and password. */
		std::string username_encrypted{Crypto::encrypt(username)};
		std::string username_encrypted_b64{Crypto::to_base64(username_encrypted)};
		std::string pw_hashed_encrypted{Crypto::encrypt(pw_hashed)};
		std::string pw_hashed_encrypted_b64{Crypto::to_base64(pw_hashed_encrypted)};

		/* Build 'select user' statement. */
		auto select_user_stmt = QueryStmt::create()
				.set_sql_query(Sql::get_query_select_user())
				.prepare(db)
				.add_param(1, QueryParam(username_encrypted_b64))
				.add_param(2, QueryParam(pw_hashed_encrypted_b64))
				.build();

		rc = sqlite3_step(select_user_stmt.stmt_);

		/* Early return with error code in case login failed. */
		if (rc != SQLITE_ROW) {
			std::cout << "Login failed.  Good bye.\n";
			return 1;
		}

		/* Retrieve user's data and assign to local user struct. */
		int user_id_from_db = (int) sqlite3_column_int(select_user_stmt.stmt_, 0);
		std::string username_from_db_b64 = (const char*) sqlite3_column_text(select_user_stmt.stmt_, 1);
		/* Decrypt user's data. */
		std::string username_from_db_raw_binary{Crypto::to_raw_binary(username_from_db_b64)};
		std::string username_from_db_decrypted{Crypto::decrypt(username_from_db_raw_binary)};

		logged_in_user.id = user_id_from_db;
		logged_in_user.username = username_from_db_decrypted;
		std::cout << "Login successful!  Welcome, " << logged_in_user.username
				<< " [ID: " << logged_in_user.id << "]\n";

	}

	/* Create user. */
	if (user_choice == "1") {

		/* Enter credentials.  Also, extract this. */
		std::cout << "Enter username: ";
		std::string username{};
		std::getline(std::cin, username);
		std::string pw_cstring = getpass("Enter password: ");
		std::string pw{pw_cstring};
		std::string pw_hashed{Crypto::sha256(pw)};
		/* Encrypt username and password. */
		std::string username_encrypted{Crypto::encrypt(username)};
		std::string username_encrypted_b64{Crypto::to_base64(username_encrypted)};
		std::string pw_hashed_encrypted{Crypto::encrypt(pw_hashed)};
		std::string pw_hashed_encrypted_b64{Crypto::to_base64(pw_hashed_encrypted)};

		/* Create user. */
		auto insert_user_stmt = QueryStmt::create()
				.set_sql_query(Sql::get_query_insert_user())
				.prepare(db)
				.add_param(1, QueryParam(username_encrypted_b64))
				.add_param(2, QueryParam(pw_hashed_encrypted_b64))
				.build();

		rc = sqlite3_step(insert_user_stmt.stmt_);

		if (rc = SQLITE_DONE) {

			/* Fetch user from DB. */
			auto fetch_user_stmt = QueryStmt::create()
					.set_sql_query(Sql::get_query_fetch_user())
					.prepare(db)
					.add_param(1, QueryParam(username_encrypted_b64))
					.build();

			rc = sqlite3_step(fetch_user_stmt.stmt_);

			/* Assign user ID from DB to local user struct member. */
			int user_id_from_db = (int) sqlite3_column_int(fetch_user_stmt.stmt_, 0);
			logged_in_user.username = username;
			logged_in_user.id = user_id_from_db;
			std::cout << "New user [ID " << logged_in_user.id << "] created.  "
					<< "Hello, " << logged_in_user.username << '\n';
		} else {
			std::cout << "Something went wrong.  Bye.\n";
			return 1;
		}
	}

	/* Check if task table exists.  If not, create it. */
	if (!DB::exists_table(db, Sql::get_query_check_for_task_table())) {
		if (!DB::create_table(db, Sql::get_query_create_task_table())) {
			std::cout << "Error: creating task table failed\n";
			return 1;
		}
	}

	/* (Replace with Builder pattern.) */
	auto main_menu = Menu::create("Create a new task", "Show all tasks", "Delete a task [ToDo]", "Exit program");

	while (true) {

		/* Show menu and get user's choice. */
		std::string choice = Input::get_menu_option_choice(main_menu, "Main menu");

		/* Store task in DB. */
		if (choice == "0") {

			/* Get task data from user. */
			std::cout << "Enter task name (max. 16 characters):\n";
			std::string task_name{};
			std::getline(std::cin, task_name);
			std::cout << "Enter task description:\n";
			std::string task_description{};
			std::getline(std::cin, task_description);

			/* Encrypt task name and description. */
			std::string task_name_raw_binary{Crypto::encrypt(task_name)};
			std::string task_name_b64{Crypto::to_base64(task_name_raw_binary)};
			std::string task_descr_raw_binary{Crypto::encrypt(task_description)};
			std::string task_descr_b64{Crypto::to_base64(task_descr_raw_binary)};

			/* Insert task. */
			auto insert_task_stmt = QueryStmt::create()
					.set_sql_query(Sql::get_query_insert_task())
					.prepare(db)
					.add_param(1, QueryParam(task_name_b64))
					.add_param(2, QueryParam(task_descr_b64))
					.add_param(3, QueryParam(logged_in_user.id))
					.build();

			rc = sqlite3_step(insert_task_stmt.stmt_);
			std::cout << "[Insert task rc: " << rc << "]\n";
		}

		/* Show all tasks by user. */
		if (choice == "1") {

			/* Fetch tasks from DB. */
			auto select_tasks_stmt = QueryStmt::create()
					.set_sql_query(Sql::get_query_select_all_tasks())
					.prepare(db)
					.add_param(1, QueryParam(logged_in_user.id))
					.build();
			
			/* Create vector of tasks. */
			std::vector<Task> all_tasks;
			while (sqlite3_step(select_tasks_stmt.stmt_) == SQLITE_ROW) {
				auto task = Task::create()
						.set("id", select_tasks_stmt.stmt_, 0)
						.set("name", select_tasks_stmt.stmt_, 1) /* Decrypted. */
						.set("description", select_tasks_stmt.stmt_, 2) /* Decrypted. */
						.set("priority", select_tasks_stmt.stmt_, 3)
						.set("user_id", select_tasks_stmt.stmt_, 4)
						.set("created_at", select_tasks_stmt.stmt_, 5)
						.build();
				all_tasks.push_back(task);
			};
			std::cout << "Number of tasks: " << all_tasks.size() << '\n';

			/* Output all tasks. */
			for (int i = 0; i < all_tasks.size(); ++i) {
				std::cout << "Task #" << all_tasks[i].get_id() << ":\t"
						<< all_tasks[i].get_created_at() << '\t'
						<< Text::crop_task_name(all_tasks[i].get_name()) << '\n';
			}

		}

		/* Quit program. */
		if (choice == "3") {
			sqlite3_close(db);
			return 0;
		}

	}

}
