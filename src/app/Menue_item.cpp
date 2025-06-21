#include "Menue_item.h"
#include <string>

Menue_item::Menue_item(int number, std::string name)
	: option_number_{number}
	, name_{name} {}

int Menue_item::get_option_number()
{
	return option_number_;
}

void Menue_item::set_option_number(int number)
{
	option_number_ = number;
}

std::string Menue_item::get_name()
{
	return name_;
}

void Menue_item::set_name(std::string name)
{
	name_ = name;
}
