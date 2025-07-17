#include "./System.h"
#include <libgen.h> /* Needed for dirname function. */
#include <string>
#include <unistd.h>
#include <vector>

std::string System::get_binary_dir()
{
	/*
	 * Non-portable, Linux-specific way of getting the directory
	 * in which the binary is located.  This ensures that the DB
	 * will be created in that same directory, not the directory
	 * from which the binary is called for execution.
	 */
	auto buffer = std::vector<char>(1024);
	const ssize_t len =
        readlink("/proc/self/exe", buffer.data(), buffer.size() - 1);
	buffer[len] = '\0'; /* Terminate the path. */
	const std::string binary_path{buffer.data()}; /* Create a copy. */
	return dirname(buffer.data()); /* Modifies buffer.data() in place. */
}
