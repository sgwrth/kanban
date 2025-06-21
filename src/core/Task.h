#ifndef TASK_H
#define TASK_H
#include <string>

class Task {
public:
	std::string get_name();
	void set_name(std::string name);
private:
	std::string name_;
};

#endif
