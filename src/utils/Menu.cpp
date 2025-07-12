#include "./Menu.h"
#include <string>

std::vector<Menu_item>::const_iterator Menu::begin() const
{
	return options_.begin();
}

std::vector<Menu_item>::const_iterator Menu::end() const
{
	return options_.end();
}

Menu Menu::create()
{
	return Menu();
}

Menu& Menu::add_option(std::string option)
{
	int index = options_.size(); /* Get index for new menu item. */
	options_.push_back(Menu_item{index, option});
	return *this;
}

Menu Menu::build()
{
	return *this;
}
