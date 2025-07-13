#include "./Output.h"
#include <iostream>
#include <vector>
#include "../core/Task.h"
#include "../utils/Text.h"

void Output::print_tasks(std::vector<Task>& tasks)
{
	for (int i = 0; i < tasks.size(); ++i) {
		std::cout << "Task #"
				<< tasks[i].get_id() << ":\t"
				<< tasks[i].get_created_at() << '\t'
				<< '[' << Text::priority_to_string(tasks[i].get_priority()) << "]\t"
				<< Text::crop_task_name(tasks[i].get_name()) << '\n';
	}
}

void Output::print_task(Task& task)
{
	std::cout << "Task ID:\t" << task.get_id() << '\n'
			<< "Created at:\t" << task.get_created_at() << '\n'
			<< "Priority:\t" << Text::priority_to_string(task.get_priority()) << '\n'
			<< "Name:\t\t" << task.get_name() << '\n'
			<< "Description:\n"
			<< task.get_description() << '\n';
}
