#ifndef INPUT_H
#define INPUT_H
#include "./Menu.h"
#include "../app/Menu_item.h"
#include <memory>
#include <string>
#include <vector>

class Input {
public:
	static std::string get_menu_option_choice(
        const Menu menu,
        const std::string& menu_name
    );
	static bool is_valid_menu_option(
        const std::string& input,
        const Menu& menu
    );
	static std::string get_selected_option_name(
        const int choice,
        const Menu& menu
    );
    static void prompt_for_enter();
};

#endif
