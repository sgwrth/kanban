#include <csignal>
#include <curses.h>
#include <iostream>
#include <string>
#include <vector>
#include "../app/Credentials.h"
#include "../app/Tui.h"
#include "../core/Task.h"
#include "../core/User.h"
#include "../utils/Crypto.h"
#include "../utils/DB.h"
#include "../utils/Input.h"
#include "../utils/Menu.h"
#include "../utils/Output.h"
#include "../utils/QueryStmt.h"
#include "../utils/Sql.h"
#include "../utils/System.h"
#include "../../external/sqlite/sqlite3.h"

int main()
{
	/* Prepare filepath for DB creation. */
	std::string db_filename{"data.db"};
	const std::string db_fullpath = System::get_binary_dir() + '/' + db_filename;

	/* Open or, if it doesn't exist, create DB. */
	sqlite3* db{nullptr};
	if (sqlite3_open(db_fullpath.c_str(), &db) != SQLITE_OK) {
		std::cerr << "Could not open DB: " << sqlite3_errmsg(db) << '\n';
	}

	if (!DB::exists_table(db, Sql::get_query_check_for_user_table())) {
		if (!DB::create_table(db, Sql::get_query_create_user_table())) {
			std::cout << "Error: creating user table failed.\n";
			return 1;
		}
	}
	
	/* User menu. */
	const std::string LOG_IN{"Log in"};
	const std::string CREATE_USER{"Create user"};
	const std::string EXIT_FROM_USER_MENU{"Exit"};

	Menu user_menu = Menu::create()
			.add_option(LOG_IN)
			.add_option(CREATE_USER)
			.add_option(EXIT_FROM_USER_MENU)
			.build();

	User logged_in_user;

	(void) initscr();
	(void) signal(SIGINT, Tui::finish);	/* arrange interrupts to terminate */
	keypad(stdscr, TRUE);			/* enable keyboard mapping */
	(void) nonl();				/* tell curses not to do NL->CR/NL on output */
	// (void) cbreak();			/* take input chars one at a time, no wait for \n */
	(void) echo();
	
	/* Show user menu and get user's choice. */
	std::string user_choice = Input::get_menu_option_choice(user_menu, "User menu"); 
	std::string selected_option_user_menu{Input::get_selected_option_name(std::stoi(user_choice), user_menu)};	

	if (selected_option_user_menu == LOG_IN) {

		Credentials creds = Credentials::get_creds_from_input();
		Credentials creds_encrypted_b64 = Crypto::encrypt_creds(creds);

		QueryStmt select_user_stmt = QueryStmt::create()
				.set_sql_query(Sql::get_query_select_user())
				.prepare(db)
				.add_param(1, QueryParam(creds_encrypted_b64.username_))
				.add_param(2, QueryParam(creds_encrypted_b64.pw_hashed_))
				.build();

		/* Early return with error code in case login failed. */
		if (sqlite3_step(select_user_stmt.stmt_) != SQLITE_ROW) {
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
		/*
		std::cout << "Login successful!  Welcome, " << logged_in_user.username
				<< " [ID: " << logged_in_user.id << "]\n";
		*/
		printw("Login successful!  Welcome, %s [ID: %d]\n",
	 		logged_in_user.username.c_str(),
	 		logged_in_user.id
	 	);
		addstr("Press ENTER to continue.");

		/* Using blocking wgetnstr() in order to display above notice. */
		noecho();
		char buff[1];
		wgetnstr(stdscr, buff, sizeof(buff) - 1);
		echo();
	}

	if (selected_option_user_menu == CREATE_USER) {

		Credentials creds = Credentials::get_creds_from_input();
		Credentials creds_encrypted_b64 = Crypto::encrypt_creds(creds);

		QueryStmt insert_user_stmt = QueryStmt::create()
				.set_sql_query(Sql::get_query_insert_user())
				.prepare(db)
				.add_param(1, QueryParam(creds_encrypted_b64.username_))
				.add_param(2, QueryParam(creds_encrypted_b64.pw_hashed_))
				.build();

		int rc = sqlite3_step(insert_user_stmt.stmt_);

		if (rc == SQLITE_DONE) {

			QueryStmt fetch_user_stmt = QueryStmt::create()
					.set_sql_query(Sql::get_query_fetch_user())
					.prepare(db)
					.add_param(1, QueryParam(creds_encrypted_b64.username_))
					.build();

			rc = sqlite3_step(fetch_user_stmt.stmt_);

			/* Assign user ID and username from DB to local user struct member. */
			int user_id_from_db = (int) sqlite3_column_int(fetch_user_stmt.stmt_, 0);
			std::string username_encrypted = (const char*) sqlite3_column_text(fetch_user_stmt.stmt_, 1);
			std::string username = Crypto::decrypt_from_b64(username_encrypted);
			logged_in_user.username = username;
			logged_in_user.id = user_id_from_db;

			std::cout << "New user [ID " << logged_in_user.id << "] created.  "
					<< "Hello, " << logged_in_user.username << '\n';
		} else {
			std::cout << "Something went wrong.  Bye.\n";
			return 1;
		}
	}

	if (selected_option_user_menu == EXIT_FROM_USER_MENU) {
		std::cout << "Bye.\n";
		sqlite3_close(db);
		Tui::finish(0);
		return 0;
	}

	if (!DB::exists_table(db, Sql::get_query_check_for_task_table())) {
		if (!DB::create_table(db, Sql::get_query_create_task_table())) {
			std::cout << "Error: creating task table failed.\n";
			return 1;
		}
	}

	/* Main menu. */
	const std::string CREATE_TASK{"Create a new task"};
	const std::string SHOW_TASKS{"Show all tasks"};
	const std::string SELECT_TASK{"Select task [ToDo]"};
	const std::string DELETE_TASK{"Delete a task [ToDo]"};
	const std::string EXIT{"Exit program"};

	Menu main_menu = Menu::create()
			.add_option(CREATE_TASK)
			.add_option(SHOW_TASKS)
			.add_option(SELECT_TASK)
			.add_option(DELETE_TASK)
			.add_option(EXIT)
			.build();

	while (true) {

		/* Show menu and get user's choice. */
		std::string choice = Input::get_menu_option_choice(main_menu, "Main menu");

		/* Get name of selected option. */
		std::string selected_option{Input::get_selected_option_name(std::stoi(choice), main_menu)};	

		clear();
		
		if (selected_option == CREATE_TASK) {

			int y_pos = 0;
			int x_pos = 0;

			/* Get task name. */
			addstr("Enter task name (max. 16 characters):");
			move(++y_pos, 0);
			char buffer_task_name[128];
			wgetnstr(stdscr, buffer_task_name, sizeof(buffer_task_name) - 1);
			std::string task_name{buffer_task_name};
			move(++y_pos, 0);

			/* Get task descripition. */
			addstr("Enter task description:");
			move(++y_pos, 0);
			char buffer_task_description[1024];
			wgetnstr(stdscr, buffer_task_description, sizeof(buffer_task_description) - 1);
			std::string task_description{buffer_task_description};

			/* Encrypt task name and description. */
			std::string task_name_raw_binary{Crypto::encrypt(task_name)};
			std::string task_name_b64{Crypto::to_base64(task_name_raw_binary)};
			std::string task_descr_raw_binary{Crypto::encrypt(task_description)};
			std::string task_descr_b64{Crypto::to_base64(task_descr_raw_binary)};

			/* Insert task. */
			QueryStmt insert_task_stmt = QueryStmt::create()
					.set_sql_query(Sql::get_query_insert_task())
					.prepare(db)
					.add_param(1, QueryParam(task_name_b64))
					.add_param(2, QueryParam(task_descr_b64))
					.add_param(3, QueryParam(logged_in_user.id))
					.build();

			int rc = sqlite3_step(insert_task_stmt.stmt_);
		}

		if (selected_option == SHOW_TASKS) {

			/* Fetch tasks from DB. */
			QueryStmt select_tasks_stmt = QueryStmt::create()
					.set_sql_query(Sql::get_query_select_all_tasks())
					.prepare(db)
					.add_param(1, QueryParam(logged_in_user.id))
					.build();
			
			/* Create vector of tasks. */
			std::vector<Task> all_tasks;
			while (sqlite3_step(select_tasks_stmt.stmt_) == SQLITE_ROW) {
				Task task = Task::create()
						.set("id", select_tasks_stmt.stmt_, 0)
						.set("name", select_tasks_stmt.stmt_, 1) /* Decrypted. */
						.set("description", select_tasks_stmt.stmt_, 2) /* Decrypted. */
						.set("priority", select_tasks_stmt.stmt_, 3) /* From int. */
						.set("user_id", select_tasks_stmt.stmt_, 4)
						.set("created_at", select_tasks_stmt.stmt_, 5)
						.build();
				all_tasks.push_back(task);
			};

			/* Output all tasks. */
			Output::print_tasks(all_tasks);
		}

		if (selected_option == SELECT_TASK) {

			std::string task_number{};
			std::cout << "Enter task #: ";
			std::getline(std::cin, task_number);

			/* Fetch task from DB. */
			QueryStmt select_task_stmt = QueryStmt::create()
					.set_sql_query(Sql::get_query_select_task())
					.prepare(db)
					.add_param(1, QueryParam(std::stoi(task_number)))
					.build();

			if (sqlite3_step(select_task_stmt.stmt_) != SQLITE_ROW) {
				std::cout << "Error fetching task\n";
				return 1;
			}
			Task task = Task::create()
				.set("id", select_task_stmt.stmt_, 0)
				.set("name", select_task_stmt.stmt_, 1) /* Decrypted. */
				.set("description", select_task_stmt.stmt_, 2) /* Decrypted. */
				.set("priority", select_task_stmt.stmt_, 3) /* From int. */
				.set("user_id", select_task_stmt.stmt_, 4)
				.set("created_at", select_task_stmt.stmt_, 5)
				.build();

			Output::print_task(task);
		}

		/* Quit program. */
		if (selected_option == EXIT) {
			sqlite3_close(db);
			Tui::finish(0);
			return 0;
		}

	}

}
