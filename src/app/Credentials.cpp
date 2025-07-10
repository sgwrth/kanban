#include "./Credentials.h"
#include <iostream>
#include <string>
#include <unistd.h>
#include "../utils/Crypto.h"

Credentials::Credentials(std::string username, std::string pw_hashed)
		: username_{username}, pw_hashed_{pw_hashed}
{}

Credentials Credentials::get_creds_from_input()
{
	std::cout << "Enter username: ";
	std::string username{};
	std::getline(std::cin, username);
	std::string pw_cstring = getpass("Enter password: ");
	std::string password{pw_cstring};
	std::string pw_hashed{Crypto::sha256(password)};
	return Credentials(username, pw_hashed);
}
