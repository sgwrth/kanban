#include "Text.h"
#include <array>
#include <string>

std::string Text::crop_task_name(std::string task_name)
{
	constexpr const int cropped_task_name_len = 19;
	constexpr const int ellipsis_len = 3;
	constexpr const char ellipsis_char = '.';
	if (task_name.size() <= (cropped_task_name_len - ellipsis_len)) {
		return task_name;
	}
	std::array<char, cropped_task_name_len> cropped_task_name{};
	for (int i = 0; i < (cropped_task_name_len - ellipsis_len); ++i) {
		cropped_task_name[i] = task_name[i];
	}
	for (int i = (cropped_task_name_len - ellipsis_len); i < cropped_task_name_len; ++i) {
		cropped_task_name[i] = ellipsis_char;
	}
	return std::string(cropped_task_name.data(), cropped_task_name.size());
}

std::string Text::priority_to_string(const Priority& priority)
{
	switch (priority) {
	case Priority::low:
		return "low";
	case Priority::medium:
		return "medium";
	case Priority::high:
		return "high";
	default:
		return "Error: invalid priority value.";
	}
}
