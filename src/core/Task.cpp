#include "Task.h"
#include <iostream>
#include <string>
#include "../core/Priority.h"
#include "../utils/Crypto.h"
#include "../../external/sqlite/sqlite3.h"

Task::Task(
    int id,
    std::string name,
    std::string description,
    int priority,
    int user_id,
    std::string created_at
)
    : id_{id}
    , name_{name}
    , description_{description}
    , priority_{priority}
    , user_id_{user_id}
    , created_at_{created_at}
{}

int Task::get_id()
{
	return id_;
}

void Task::set_id(int id)
{
	id_ = id;
}

std::string Task::get_name()
{
	return name_;
}

void Task::set_name(const std::string name)
{
	name_ = name;
}

std::string Task::get_description()
{
	return description_;
}

void Task::set_description(const std::string description)
{
	description_ = description;
}

Priority Task::get_priority() const
{
	return priority_;
}

void Task::set_priority(const Priority priority)
{
	priority_ = priority;
}

int Task::get_user_id()
{
	return user_id_;
}

void Task::set_user_id(int user_id)
{
	user_id_ = user_id;
}

std::string Task::get_created_at()
{
	return created_at_;
}

void Task::set_created_at(std::string datetime)
{
	created_at_ = datetime;
}


/* Builder. */

Task Task::create()
{
	return Task(-1, "", "", -1, -1, "");
}

Task& Task::set(std::string member, sqlite3_stmt* stmt, int column)
{
	if (member == "id") {
		int id = (int) sqlite3_column_int(stmt, column);
		set_id(id);

	} else if (member == "name") {
		std::string name = (const char*) sqlite3_column_text(stmt, column);
		std::string name_raw_binary = Crypto::to_raw_binary(name);
		std::string name_decrypted = Crypto::decrypt(name_raw_binary);
		set_name(name_decrypted);

	} else if (member == "description") {
		std::string description =
            (const char*) sqlite3_column_text(stmt, column);
		std::string description_raw_binary =
            Crypto::to_raw_binary(description);
		std::string description_decrypted =
            Crypto::decrypt(description_raw_binary);
		set_description(description_decrypted);

	} else if (member == "priority") {
		int priority = (int) sqlite3_column_int(stmt, column);
		switch (priority) {
		case 0:
			set_priority(Priority::LOW);
			break;
		case 1:
			set_priority(Priority::MEDIUM);
			break;
		case 2:
			set_priority(Priority::HIGH);
			break;
		default:
			std::cout << "Error: invalid priority.\n";
			break;
		}

	} else if (member == "user_id") {
		int user_id = (int) sqlite3_column_int(stmt, column);
		set_user_id(user_id);

	} else if (member == "created_at") {
		std::string created_at =
            (const char*) sqlite3_column_text(stmt, column);
		set_created_at(created_at);

	} else {
		std::cout << "Error: invalid member.\n";
	}

	return *this;
}

Task Task::build()
{
	return *this;
}
