#include "Task.h"
#include <string>
#include "../core/Priority.h"

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

std::string Task::get_created_at()
{
	return created_at_;
}
