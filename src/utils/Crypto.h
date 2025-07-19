#ifndef ENCRYPT_H
#define ENCRYPT_H
#include <string>
#include "../app/Credentials.h"

class Crypto {
public:
	static std::string sha256(const std::string& str);
	static std::string encrypt(const std::string& plaintext);
	static std::string decrypt(const std::string& ciphertext);
	static std::string to_base64(const std::string& raw_binary);
	static std::string to_bin(const std::string& base64);
	static Credentials encrypt_creds(Credentials creds);
	static std::string decrypt_from_b64(std::string base_64);
};

#endif
