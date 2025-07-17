#ifndef TEXT_H
#define TEXT_H
#include <string>
#include "../core/Priority.h"

class Text {
public:
	static std::string crop_task_name(const std::string& task_name);
	static std::string priority_to_str(const Priority& priority);
};

#endif
