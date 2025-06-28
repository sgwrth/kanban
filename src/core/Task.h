#ifndef TASK_H
#define TASK_H
#include <string>
#include "../core/Priority.h"

class Task {
public:
	Task(int id, std::string name, std::string description, int priority, int user_id, std::string created_at);
	int get_id();
	std::string get_name();
	void set_name(const std::string name);
	std::string get_description();
	void set_description(const std::string description);
	Priority get_priority();
	void set_priority(const Priority priority);
	int get_user_id();
	std::string get_created_at();
private:
	int id_;
	std::string name_;
	std::string description_;
	Priority priority_;
	int user_id_;
	std::string created_at_;
};

#endif
