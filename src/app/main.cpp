#include <iostream>
#include <memory>
#include "../core/Task.h"

int main()
{
	std::cout << "helloworld\n";
	auto task0 = std::make_unique<Task>();
	task0->set_name("task 0");
	std::cout << task0->get_name() << "\n";
}
