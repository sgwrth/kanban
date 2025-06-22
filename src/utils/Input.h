#ifndef INPUT_H
#define INPUT_H
#include "../app/Menu_item.h"
#include <memory>
#include <string>
#include <vector>

class Input {
public:
	static bool is_valid_menu_option(
			const std::string& input,
			const std::vector<std::unique_ptr<Menu_item>>& menu
	);
};

#endif
