#include "Task.h"
#include <string>
#include "../core/Priority.h"

Task::Task(int id, std::string name, std::string description, int priority, int user_id, std::string created_at)
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

Priority Task::get_priority()
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


std::string Task::get_created_at()
{
	return created_at_;
}
