#ifndef ENCRYPT_H
#define ENCRYPT_H
#include <string>

class Encrypt {
public:
	static std::string sha256(const std::string& str);
};

#endif
