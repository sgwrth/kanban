#include "Task.h"
#include <string>

std::string Task::get_name()
{
	return name_;
}

void Task::set_name(std::string name)
{
	name_ = name;
}
