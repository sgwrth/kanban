#include "Input.h"
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include "../app/Menu_item.h"

std::string Input::get_menu_option_choice(
		const std::vector<std::unique_ptr<Menu_item>>& menu,
		const std::string& menu_name
)
{
	std::string user_choice {};
	do {
		std::cout << menu_name << ":\n";
		for (const auto& option : menu) {
			std::cout << "[" << option->get_number() << "] "
					<< option->get_name() + "\n";
		}
		std::cout << "Choice: ";
		std::getline(std::cin, user_choice);
	} while (!Input::is_valid_menu_option(user_choice, menu));
	return user_choice;
}

bool Input::is_valid_menu_option(
		const std::string& input,
		const std::vector<std::unique_ptr<Menu_item>>& menu
)
{
	try {
		std::size_t input_size;
		int choice = std::stoi(input, &input_size);
		return (input_size == input.length())
				&& (choice >= 0)
				&& (choice < menu.size());
	} catch (const std::invalid_argument&) {
		return false;
	} catch (const std::out_of_range&) {
		return false;
	}
}

