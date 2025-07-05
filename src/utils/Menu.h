#ifndef MENU_H
#define MENU_H
#include <memory>
#include <vector>
#include "../app/Menu_item.h"

class Menu {
public:
	template<typename... Args>
	static std::vector<std::unique_ptr<Menu_item>> create(Args... menu_options)
	{
		std::vector<std::string> user_menu_options{menu_options...};
		std::vector<std::unique_ptr<Menu_item>> user_menu;
		for (int i = 0; i < user_menu_options.size(); ++i) {
			user_menu.push_back(std::make_unique<Menu_item>(i, user_menu_options[i]));
		}
		return user_menu;
	}
};

#endif
