#include "../utils/Crypto.h"
#include <algorithm>
#include <cryptopp/aes.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/modes.h>
#include <iomanip>
#include <openssl/sha.h>
#include <sstream>
#include "../secret/KeyIV.h"

std::string Crypto::sha256(const std::string& str)
{
	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256((const unsigned char*) str.c_str(), str.size(), hash);

	std::stringstream ss;
	for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
	ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
	return ss.str();
}

std::string Crypto::encrypt(std::string plaintext)
{
	KeyIV key_and_iv;
	CryptoPP::byte key[CryptoPP::AES::DEFAULT_KEYLENGTH];
	CryptoPP::byte iv[CryptoPP::AES::BLOCKSIZE];
	std::copy(std::begin(key_and_iv.key), std::end(key_and_iv.key), key);
	std::copy(std::begin(key_and_iv.iv), std::end(key_and_iv.iv), iv);

	std::string ciphertext;
	CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption encryptor;
	encryptor.SetKeyWithIV(key, sizeof(key), iv);
	CryptoPP::StringSource string_source_encrypt(
			plaintext,
			true,
			new CryptoPP::StreamTransformationFilter(
					encryptor,
					new CryptoPP::StringSink(ciphertext)
			)
	);
	return ciphertext;
}
