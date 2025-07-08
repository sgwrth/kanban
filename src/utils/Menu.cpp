#include "./Menu.h"
#include <string>

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
