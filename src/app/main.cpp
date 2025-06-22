#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "../app/Menu_item.h"
#include "../core/Task.h"
#include "../utils/Input.h"

int main()
{
	std::vector<std::string> menu_options;
	menu_options.push_back("Create a new task");
	menu_options.push_back("Show all tasks");
	menu_options.push_back("Delete a task");

	std::vector<std::unique_ptr<Menu_item>> menu;
	for (int i = 0; i < menu_options.size(); ++i) {
		menu.push_back(std::make_unique<Menu_item>(i, menu_options[i]));
	}

	std::string choice {};
	do {
		std::cout << "Menu:\n";
		for (const auto& option : menu) {
			std::cout << "[" << option->get_number() << "] " << option->get_name() + "\n";
		}
		std::cout << "Choice: \n";
		std::cin.clear();
		std::cin >> choice;
	} while (!Input::is_valid_menu_option(choice, menu));
}
