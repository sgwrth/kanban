#include <iostream>
#include <memory>
#include <vector>
#include "../app/Menue_item.h"
#include "../core/Task.h"

int main()
{
	std::cout << "helloworld\n";
	const auto task0 = std::make_unique<Task>();
	task0->set_name("task 0");
	std::cout << task0->get_name() << "\n";

	std::vector<std::string> menue_options;
	menue_options.push_back("Create a new task");
	menue_options.push_back("Show all tasks");
	menue_options.push_back("Delete a task");

	std::vector<std::unique_ptr<Menue_item>> menue;
	for (int i = 0; i < menue_options.size(); ++i) {
		menue.push_back(std::make_unique<Menue_item>(i, menue_options[i]));
	}

	auto choice {-1};
	std::cin >> choice;
	// do {
	// std::cout << "menu:\n";
	// std::cout << "[0] create new task\n";
	// std::cout << "[1] show tasks\n";
	// std::cout << "[2] delete task\n";
	// } while (invalid_choice(choice));
}
