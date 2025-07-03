#include <iostream>
#include <libgen.h> /* Needed for dirname function. */
#include <memory>
#include <string>
#include <unistd.h>
#include <vector>
#include "../app/Menu_item.h"
#include "../core/Task.h"
#include "../core/User.h"
#include "../utils/Crypto.h"
#include "../utils/Input.h"
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

	/* Check if user table exists. */
	std::string check_for_user_table = Sql::get_query_check_for_user_table();
	sqlite3_stmt* check_user_stmt{nullptr};
	rc = sqlite3_prepare_v2(db, check_for_user_table.c_str(), -1, &check_user_stmt, 0);
	rc = sqlite3_step(check_user_stmt);

	/* No user table found.  So, create it. */
	if (rc != SQLITE_ROW) {
		std::string create_user_table = Sql::get_query_create_user_table();
		rc = sqlite3_prepare_v2(db, create_user_table.c_str(), -1, &check_user_stmt, 0);
		rc = sqlite3_step(check_user_stmt);
	}
	
	/* Create user menu. */
	std::vector<std::string> user_menu_options;
	user_menu_options.push_back("Log in");
	user_menu_options.push_back("Create user");
	std::vector<std::unique_ptr<Menu_item>> user_menu;
	for (int i = 0; i < user_menu_options.size(); ++i) {
		user_menu.push_back(std::make_unique<Menu_item>(i, user_menu_options[i]));
	}

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

		/* Fetching user from DB.  If user exists, get the 'username' column. */
		std::string select_user = Sql::get_query_select_user();
		sqlite3_stmt* select_user_stmt{nullptr};
		rc = sqlite3_prepare_v2(db, select_user.c_str(), -1, &select_user_stmt, 0);
		rc = sqlite3_bind_text(select_user_stmt, 1, username_encrypted_b64.c_str(), -1, SQLITE_STATIC);
		rc = sqlite3_bind_text(select_user_stmt, 2, pw_hashed_encrypted_b64.c_str(), -1, SQLITE_STATIC);
		rc = sqlite3_step(select_user_stmt);

		/* Early return with error code in case login failed. */
		if (rc != SQLITE_ROW) {
			std::cout << "Login failed.  Good bye.\n";
			return -1;
		}

		/* Retrieve user's data and assign to local user struct. */
		int user_id_from_db = (int) sqlite3_column_int(select_user_stmt, 0);
		std::string username_from_db_b64 = (const char*) sqlite3_column_text(select_user_stmt, 1);
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
		sqlite3_stmt* insert_user_stmt{nullptr};
		std::string insert_user = Sql::get_query_insert_user();
		rc = sqlite3_prepare_v2(db, insert_user.c_str(), -1, &insert_user_stmt, 0);
		rc = sqlite3_bind_text(insert_user_stmt, 1, username_encrypted_b64.c_str(), -1, SQLITE_STATIC);
		rc = sqlite3_bind_text(insert_user_stmt, 2, pw_hashed_encrypted_b64.c_str(), -1, SQLITE_STATIC);
		rc = sqlite3_step(insert_user_stmt);
		if (rc = SQLITE_DONE) {

			/* Fetch user from DB. */
			std::string fetch_user = Sql::get_query_fetch_user();
			sqlite3_stmt* fetch_user_stmt{nullptr};
			rc = sqlite3_prepare_v2(db, fetch_user.c_str(), -1, &fetch_user_stmt, 0);
			rc = sqlite3_bind_text(fetch_user_stmt, 1, username_encrypted_b64.c_str(), -1, SQLITE_STATIC);
			rc = sqlite3_step(fetch_user_stmt);

			/* Assign user ID from DB to local user struct member. */
			int user_id_from_db = (int) sqlite3_column_int(fetch_user_stmt, 0);
			logged_in_user.username = username;
			logged_in_user.id = user_id_from_db;
			std::cout << "New user [ID " << logged_in_user.id << "] created.  "
					<< "Hello, " << logged_in_user.username << '\n';
		} else {
			std::cout << "Something went wrong.  Bye.\n";
			return 1;
		}
	}

	/* Check if task table exists. */
	std::string check_for_task_table = Sql::get_query_check_for_task_table();
	sqlite3_stmt* check_task_stmt{nullptr};
	rc = sqlite3_prepare_v2(db, check_for_task_table.c_str(), -1, &check_task_stmt, 0);
	rc = sqlite3_step(check_task_stmt);

	/* No task table was found.  So, create it. */
	if (rc != SQLITE_ROW) {
		std::string create_task_table = Sql::get_query_create_task_table();
		rc = sqlite3_prepare_v2(db, create_task_table.c_str(), -1, &check_task_stmt, 0);
		rc = sqlite3_step(check_task_stmt);
	}

	/* Create main menu. */
	std::vector<std::string> main_menu_options;
	main_menu_options.push_back("Create a new task");
	main_menu_options.push_back("Show all tasks");
	main_menu_options.push_back("Delete a task [Todo]");
	main_menu_options.push_back("Exit program");
	std::vector<std::unique_ptr<Menu_item>> main_menu;
	for (int i = 0; i < main_menu_options.size(); ++i) {
		main_menu.push_back(std::make_unique<Menu_item>(i, main_menu_options[i]));
	}

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
			std::string insert_task = Sql::get_query_insert_task();
			sqlite3_stmt* insert_task_stmt{nullptr};
			rc = sqlite3_prepare_v2(db, insert_task.c_str(), -1, &insert_task_stmt, 0);
			rc = sqlite3_bind_text(insert_task_stmt, 1, task_name_b64.c_str(), -1, SQLITE_STATIC);
			rc = sqlite3_bind_text(insert_task_stmt, 2, task_descr_b64.c_str(), -1, SQLITE_STATIC);
			rc = sqlite3_bind_int(insert_task_stmt, 3, logged_in_user.id);
			rc = sqlite3_step(insert_task_stmt);
		}

		/* Show all tasks by user. */
		if (choice == "1") {

			/* Fetch tasks from DB. */
			std::string select_all_tasks = Sql::get_query_select_all_tasks();
			sqlite3_stmt* select_tasks_stmt{nullptr};
			rc = sqlite3_prepare_v2(db, select_all_tasks.c_str(), -1, &select_tasks_stmt, 0);
			rc = sqlite3_bind_int(select_tasks_stmt, 1, logged_in_user.id);
			
			/* Create vector of tasks. */
			std::vector<std::unique_ptr<Task>> all_tasks;
			while (sqlite3_step(select_tasks_stmt) == SQLITE_ROW) {
				int id = (int) sqlite3_column_int(select_tasks_stmt, 0);
				std::string name = (const char*) sqlite3_column_text(select_tasks_stmt, 1);
				std::string description = (const char*) sqlite3_column_text(select_tasks_stmt, 2);
				int priority = (int) sqlite3_column_int(select_tasks_stmt, 3);
				int user_id = (int) sqlite3_column_int(select_tasks_stmt, 4);
				std::string created_at = (const char*) sqlite3_column_text(select_tasks_stmt, 5);

				/* Decrypt data from Base64 to raw binary to plaintext. */
				std::string name_raw_binary{Crypto::to_raw_binary(name)};
				std::string name_decrypted{Crypto::decrypt(name_raw_binary)};
				std::string description_raw_binary{Crypto::to_raw_binary(description)};
				std::string description_decrypted{Crypto::decrypt(description_raw_binary)};

				auto task = std::make_unique<Task>(
						id,
						name_decrypted,
						description_decrypted,
						priority,
						user_id,
						created_at
				);
				all_tasks.push_back(std::move(task));
			};

			/* Output all tasks. */
			for (int i = 0; i < all_tasks.size(); ++i) {
				std::cout << "Task #" << all_tasks[i]->get_id() << ":\t"
						<< all_tasks[i]->get_created_at() << "\t"
						<< Text::crop_task_name(all_tasks[i]->get_name()) << '\n';
			}

		}

		/* Quit program. */
		if (choice == "3") {
			sqlite3_close(db);
			return 0;
		}

	}

}
