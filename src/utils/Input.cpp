#include "Input.h"
#include <curses.h>
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
	std::string user_choice {};
	unsigned short y_pos{0};
	int x_pos{0};
	char buffer[2];
	do {
		clear();
		y_pos = 0;
		move(y_pos, 0);
		for (const auto& option : menu.options_) {
			printw(
				"[%d] %s\n",
				option.get_number(),
				option.get_name().c_str()
			);
			move(++y_pos, 0);
		}

		/* wgetnstr() mplicitly calls refresh(). */
		wgetnstr(stdscr, buffer, sizeof(buffer) - 1);

		user_choice = buffer;
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

std::string Input::get_opt_name(
    const int choice,
    const Menu& menu
)
{
	std::string selected_option{};
	for (const auto& menu_item : menu) {
		if (menu_item.get_number() == choice) {
			return menu_item.get_name();
		}
	}
	return selected_option;
}

void Input::prompt_for_enter(std::string message)
{
    addstr(message.c_str());

    /* Using blocking wgetnstr() in order to above display notice. */
    noecho();
    char buff[2];
    wgetnstr(stdscr, buff, sizeof(buff) - 1);
    echo();
}
