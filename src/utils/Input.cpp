#include "Input.h"
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include "./Menu.h"
#include "../app/Menu_item.h"

std::string Input::get_menu_option_choice(
		const Menu menu,
		const std::string& menu_name
)
{
	std::string user_choice{};
	do {
		std::cout << menu_name << ":\n";
		for (const auto& option : menu.options_) {
			std::cout << "[" << option.get_number() << "] "
					<< option.get_name() + "\n";
		}
		std::cout << "Choice: ";
		std::getline(std::cin, user_choice);
	} while (!Input::is_valid_menu_option(user_choice, menu));
	return user_choice;
}

bool Input::is_valid_menu_option(const std::string& input, const Menu& menu)
{
	try {
		std::size_t input_size;
		int choice = std::stoi(input, &input_size);
		return (input_size == input.length())
				&& (choice >= 0)
				&& (choice < menu.options_.size());
	} catch (const std::invalid_argument&) {
		return false;
	} catch (const std::out_of_range&) {
		return false;
	}
}
std::string Input::get_selected_option_name(const int choice, const Menu& menu)
{
	std::string selected_option{};
	for (const auto& menu_item : menu) {
		if (menu_item.get_number() == choice) {
			return menu_item.get_name();
		}
	}
	return selected_option;
}


