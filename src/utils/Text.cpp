#include "Text.h"
#include <array>
#include <string>
#define TASK_NAME_AND_ELLIPSIS_LEN 19
#define ELLIPSIS_LEN 3
#define CROPPED_TASK_NAME_LEN TASK_NAME_AND_ELLIPSIS_LEN - ELLIPSIS_LEN
#define ELLIPSIS_CHAR '.'

std::string Text::crop_task_name(const std::string& task_name)
{
	if (task_name.size() <= (CROPPED_TASK_NAME_LEN)) {
		return task_name;
	}
	std::array<char, TASK_NAME_AND_ELLIPSIS_LEN> cropped_task_name{};

	/* Copy defined number of chars of task name. */
	for (int i = 0; i < (CROPPED_TASK_NAME_LEN); ++i) {
		cropped_task_name[i] = task_name[i];
	}

	/* Add ellipsis. */
	for (int i = (CROPPED_TASK_NAME_LEN); i < TASK_NAME_AND_ELLIPSIS_LEN; ++i) {
		cropped_task_name[i] = ELLIPSIS_CHAR;
	}

	return std::string(cropped_task_name.data(), cropped_task_name.size());
}

std::string Text::priority_to_string(const Priority& priority)
{
	switch (priority) {
	case Priority::LOW:
		return "Low";
	case Priority::MEDIUM:
		return "Mid";
	case Priority::HIGH:
		return "High";
	default:
		return "Error: invalid priority value.";
	}
}
