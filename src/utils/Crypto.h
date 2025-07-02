#ifndef ENCRYPT_H
#define ENCRYPT_H
#include <string>

class Crypto {
public:
	static std::string sha256(const std::string& str);
	static std::string encrypt(std::string plaintext);
	static std::string decrypt(std::string ciphertext);
};

#endif
