#ifndef TASK_H
#define TASK_H
#include <string>
#include "../core/Priority.h"

class Task {
public:
	std::string get_name();
	void set_name(const std::string name);
	std::string get_description();
	void set_description(const std::string description);
	Priority get_priority();
	void set_priority(const Priority priority);
private:
	std::string name_;
	std::string description_;
	Priority priority_;
};

#endif
