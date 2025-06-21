#ifndef MENUE_ITEM_H
#define MENUE_ITEM_H
#include <string>

class Menue_item {
public:
	Menue_item(int number, std::string name);
	int get_option_number();
	void set_option_number(int number);
	std::string get_name();
	void set_name(std::string name);
private:
	int option_number_;
	std::string name_;
};

#endif
