#include "./Output.h"
#include <iostream>
#include <vector>
#include "../core/Priority.h"
#include "../core/Task.h"
#include "../utils/Text.h"

void Output::print_tasks(std::vector<Task>& tasks)
{
	for (int i = 0; i < tasks.size(); ++i) {
		std::cout << "Task #"
				<< tasks[i].get_id() << ":\t"
				<< tasks[i].get_created_at() << '\t'
				<< Text::crop_task_name(tasks[i].get_name()) << '\t'
				<< Text::priority_to_string(tasks[i].get_priority()) << '\n';
	}
}
