#include "Menu_item.h"
#include <string>

Menu_item::Menu_item(int number, std::string name)
	: number_{number}
	, name_{name} {}

int Menu_item::get_number()
{
	return number_;
}

void Menu_item::set_number(int number)
{
	number_ = number;
}

std::string Menu_item::get_name()
{
	return name_;
}

void Menu_item::set_name(std::string name)
{
	name_ = name;
}
