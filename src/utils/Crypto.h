#ifndef ENCRYPT_H
#define ENCRYPT_H
#include <string>

class Crypto {
public:
	static std::string sha256(const std::string& str);
	static std::string encrypt(const std::string& plaintext);
	static std::string decrypt(const std::string& ciphertext);
	static std::string to_base64(const std::string& raw_binary);
	static std::string to_raw_binary(const std::string& base64);
};

#endif
