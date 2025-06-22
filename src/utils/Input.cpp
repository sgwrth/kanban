#include "Input.h"
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include "../app/Menu_item.h"

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
