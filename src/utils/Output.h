#ifndef OUTPUT_H
#define OUTPUT_H
#include <vector>
#include "../core/Task.h"

class Output {
public:
	static void print_tasks(std::vector<Task>& tasks);
	static void print_task(Task& task);
};

#endif
