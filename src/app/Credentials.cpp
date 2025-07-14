#include "./Credentials.h"
#include <curses.h>
#include <string>
#include <unistd.h>
#include "../utils/Crypto.h"

Credentials::Credentials(std::string username, std::string pw_hashed)
		: username_{username}, pw_hashed_{pw_hashed}
{}

Credentials Credentials::get_creds_from_input()
{
	int y_pos{0};
	int x_pos{0};

	char buffer_username[128];
	char buffer_password[128];
	
	clear();
	
	addstr("Enter username: ");
	std::string username{};
	getstr(buffer_username);
	username = buffer_username;

	move(++y_pos, 0);
	
	addstr("Enter password: ");
	std::string password;
	noecho();
	getstr(buffer_password);
	echo();
	password = buffer_password;
	std::string pw_hashed{Crypto::sha256(password)};

	clear();

	return Credentials(username, pw_hashed);
}
