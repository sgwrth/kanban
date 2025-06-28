#ifndef INPUT_H
#define INPUT_H
#include "../app/Menu_item.h"
#include <memory>
#include <string>
#include <vector>

class Input {
public:
	static std::string get_menu_option_choice(
			const std::vector<std::unique_ptr<Menu_item>>& menu,
			const std::string& menu_name
	);
	static bool is_valid_menu_option(
			const std::string& input,
			const std::vector<std::unique_ptr<Menu_item>>& menu
	);
};

#endif
