#include "./Output.h"
#include <curses.h>
#include <vector>
#include "../core/Task.h"
#include "../utils/Text.h"

void Output::print_tasks(std::vector<Task>& tasks)
{
    clear();
    unsigned short y_pos{0};
    addstr("Tasks:\n");
	for (int i = 0; i < tasks.size(); ++i) {
        printw(
            "#%.4d  %s  [%s]\t%s\n",
            tasks[i].get_id(),
            tasks[i].get_created_at().c_str(),
            Text::priority_to_str(tasks[i].get_priority()).c_str(),
            Text::crop_task_name(tasks[i].get_name()).c_str()
        );
        refresh();
	}
}

void Output::print_task(Task& task)
{
    clear();
    unsigned short y_pos{0};
    printw("Task ID:\t%.3d\n", task.get_id());
    printw("Created at:\t%s\n", task.get_created_at().c_str());
    printw("Priority:\t%s\n", Text::priority_to_str(task.get_priority()).c_str());
    printw("Name:\t\t%s\n", task.get_name().c_str());
    printw("Description:\n");
    printw("%s\n", task.get_description().c_str());
}
