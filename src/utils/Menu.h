#ifndef MENU_H
#define MENU_H
#include <memory>
#include <string>
#include <vector>
#include "../app/Menu_item.h"

class Menu {
public:
	std::vector<Menu_item> options_{};
	std::vector<Menu_item>::const_iterator begin() const;
	std::vector<Menu_item>::const_iterator end() const;
	static Menu create();
	Menu& add_option(std::string option);
	Menu build();
};

#endif
