#ifndef CREDENTIALS_H
#define CREDENTIALS_H
#include <string>

class Credentials {
public:
	std::string username_;
	std::string pw_hashed_;
	Credentials(std::string username, std::string pw_hashed);
	static Credentials get_creds_from_input();
};

#endif
