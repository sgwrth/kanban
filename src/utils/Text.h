#ifndef TEXT_H
#define TEXT_H
#include <string>
#include "../core/Priority.h"

class Text {
public:
	static std::string crop_task_name(std::string task_name);
	static std::string priority_to_string(const Priority& priority);
};

#endif
