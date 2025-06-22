#ifndef MENU_ITEM_H
#define MENU_ITEM_H
#include <string>

class Menu_item {
public:
	Menu_item(int number, std::string name);
	int get_number();
	void set_number(int number);
	std::string get_name();
	void set_name(std::string name);
private:
	int number_;
	std::string name_;
};

#endif
