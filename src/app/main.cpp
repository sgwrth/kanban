#include <cctype>
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

#define DB_FILENAME "data.db"

int main()
{
	/* Prepare filepath for DB creation. */
	const std::string db_fullpath = System::get_bin_dir() + '/' + DB_FILENAME;

	/* Open or, if it doesn't exist, create DB. */
	sqlite3* db{nullptr};
	if (sqlite3_open(db_fullpath.c_str(), &db) != SQLITE_OK) {
		std::cerr << "Could not open DB: " << sqlite3_errmsg(db) << '\n';
        return 1;
	}

	if (!DB::exists_table(db, Sql::check_for_user_table())) {
        /* If creation of user table fails, exit program. */
		if (!DB::create_table(db, Sql::create_user_table())) {
			std::cerr << "Error: creating user table failed.\n";
			sqlite3_close(db);
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

	User logged_in_user; /* Global variable. */

	(void) initscr();
	(void) signal(SIGINT, Tui::finish);
	keypad(stdscr, TRUE);
	(void) nonl();
	// (void) cbreak(); /* Take input one char at a time. */
	(void) echo();
	
	/* Show user menu and get user's choice. */
	std::string user_choice = Input::get_menu_option_choice(user_menu, "User menu"); 

	/* Get selected option name. */
	std::string selected_option_user_menu =
		Input::get_opt_name(std::stoi(user_choice), user_menu);

	if (selected_option_user_menu == LOG_IN) {

		Credentials creds = Credentials::get_creds_from_input();
		Credentials creds_encryp_b64 = Crypto::encrypt_creds(creds);

		QueryStmt select_user = QueryStmt::create()
			.set_sql_query(Sql::select_user())
			.prepare(db)
			.add_param(1, QueryParam(creds_encryp_b64.username_))
			.add_param(2, QueryParam(creds_encryp_b64.pw_hashed_))
			.build();

		/* Early return with error code in case login failed. */
		if (sqlite3_step(select_user.stmt_) != SQLITE_ROW) {
			std::cout << "Login failed.  Good bye.\n"; /* Use curses and close DB. */
            Tui::finish(0);
			return 1;
		}

		/* Read user's data, decrypt it and assign it to user struct. */
        DB::assign_user_data(select_user, logged_in_user);

		printw(
            "Login successful!  Welcome, %s [ID: %d]\n",
	 		logged_in_user.username.c_str(),
	 		logged_in_user.id
	 	);
        Input::prompt_for_enter("Press ENTER to continue.");
	}

	if (selected_option_user_menu == CREATE_USER) {

		Credentials creds = Credentials::get_creds_from_input();
		Credentials creds_encryp_b64 = Crypto::encrypt_creds(creds);

		QueryStmt insert_user = QueryStmt::create()
			.set_sql_query(Sql::insert_user())
			.prepare(db)
			.add_param(1, QueryParam(creds_encryp_b64.username_))
			.add_param(2, QueryParam(creds_encryp_b64.pw_hashed_))
			.build();
		
		/* Early return (exit) in case of error. */
		if (sqlite3_step(insert_user.stmt_) != SQLITE_DONE) {
			addstr("Something went wrong.\n");
            Input::prompt_for_enter("Press ENTER to exit the program.");
            sqlite3_close(db);
            Tui::finish(0);
			return 1;
		}

		QueryStmt fetch_user = QueryStmt::create()
			.set_sql_query(Sql::fetch_user())
			.prepare(db)
			.add_param(1, QueryParam(creds_encryp_b64.username_))
			.build();

		int rc = sqlite3_step(fetch_user.stmt_);

		/* Read user's data, decrypt it and assign it to user struct. */
        DB::assign_user_data(fetch_user, logged_in_user);

        printw(
            "New user [ID %d] created.  Hello, %s\n",
            logged_in_user.id, 
            logged_in_user.username.c_str()
        );
        Input::prompt_for_enter("Press ENTER to continue.");
	}

	if (selected_option_user_menu == EXIT_FROM_USER_MENU) {
		std::cout << "Bye.\n"; /* Use curses. */
		sqlite3_close(db);
		Tui::finish(0);
		return 0;
	}

	if (!DB::exists_table(db, Sql::check_for_task_table())) {
        /* If creation of task table fails, exit program. */
		if (!DB::create_table(db, Sql::create_task_table())) {
			std::cout << "Error: creating task table failed.\n";
            sqlite3_close(db);
			Tui::finish(0);
			return 1;
		}
	}

	/* Main menu. */
	const std::string CREATE_TASK{"Create a new task"};
	const std::string SHOW_TASKS{"Show all tasks"};
	const std::string SELECT_TASK{"Select task"};
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
		std::string selected_option =
            Input::get_opt_name(std::stoi(choice), main_menu);

		clear();
		
		if (selected_option == CREATE_TASK) {

			unsigned short y_pos{0};

			/* Get task name. */
			addstr("Enter task name (max. 16 characters):\n");
			char buffer_task_name[17]; /* Magic number. */
			wgetnstr(stdscr, buffer_task_name, sizeof(buffer_task_name) - 1);
			std::string task_name{buffer_task_name};
			move(++y_pos, 0);

			/* Get task description. */
			addstr("Enter task description:\n");
			char buffer_task_descr[1025]; /* Magic number. */
			wgetnstr(stdscr, buffer_task_descr, sizeof(buffer_task_descr) - 1);
			std::string task_descr{buffer_task_descr};

			/* Encrypt task name and description. */
			std::string task_name_bin{Crypto::encrypt(task_name)};
			std::string task_name_b64{Crypto::to_base64(task_name_bin)};
			std::string task_descr_bin{Crypto::encrypt(task_descr)};
			std::string task_descr_b64{Crypto::to_base64(task_descr_bin)};

			/* Insert task. */
			QueryStmt insert_task = QueryStmt::create()
				.set_sql_query(Sql::insert_task())
				.prepare(db)
				.add_param(1, QueryParam(task_name_b64))
				.add_param(2, QueryParam(task_descr_b64))
				.add_param(3, QueryParam(logged_in_user.id))
				.build();

			int rc = sqlite3_step(insert_task.stmt_);
		}

		if (selected_option == SHOW_TASKS) {
            std::vector<Task> all_tasks = DB::get_tasks(db, logged_in_user);
			Output::print_tasks(all_tasks);
            Input::prompt_for_enter("Press ENTER to continue.");
		}

		if (selected_option == SELECT_TASK) {

            clear();

            std::vector<Task> all_tasks = DB::get_tasks(db, logged_in_user);
            Output::print_tasks(all_tasks);

            char buf_task_num[5]; /* Magic number. */

            Input::get_num(buf_task_num, 5, "Enter task #: ");

            clear();
            refresh();

			/* Fetch task from DB. */
			QueryStmt select_task = QueryStmt::create()
				.set_sql_query(Sql::select_task())
				.prepare(db)
				.add_param(1, QueryParam(std::stoi(std::string(buf_task_num))))
				.build();

			if (sqlite3_step(select_task.stmt_) != SQLITE_ROW) {
                Input::prompt_for_enter("Error fetching task.  Bye.");
                sqlite3_close(db);
                Tui::finish(0);
				return 1;
			}

			Task task = Task::create()
				.set("id", select_task.stmt_, 0)
				.set("name", select_task.stmt_, 1)
				.set("description", select_task.stmt_, 2)
				.set("priority", select_task.stmt_, 3)
				.set("user_id", select_task.stmt_, 4)
				.set("created_at", select_task.stmt_, 5)
				.build();

			Output::print_task(task);
            Input::prompt_for_enter("Press ENTER to continue.");
		}

		/* Quit program. */
		if (selected_option == EXIT) {
			sqlite3_close(db);
			Tui::finish(0);
			return 0;
		}

	}

}
