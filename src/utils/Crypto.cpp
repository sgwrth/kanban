#include "../utils/Crypto.h"
#include <algorithm>
#include <cryptopp/aes.h>
#include <cryptopp/base64.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/modes.h>
#include <iomanip>
#include <openssl/sha.h>
#include <sstream>
#include "../app/Credentials.h"
#include "../secret/KeyIV.h"

std::string Crypto::sha256(const std::string& str)
{
	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256((const unsigned char*) str.c_str(), str.size(), hash);

	std::stringstream ss;
	for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
	return ss.str();
}

std::string Crypto::encrypt(const std::string& plaintext)
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

std::string Crypto::decrypt(const std::string& ciphertext)
{
	KeyIV key_and_iv;
	CryptoPP::byte key[CryptoPP::AES::DEFAULT_KEYLENGTH];
	CryptoPP::byte iv[CryptoPP::AES::BLOCKSIZE];
	std::copy(std::begin(key_and_iv.key), std::end(key_and_iv.key), key);
	std::copy(std::begin(key_and_iv.iv), std::end(key_and_iv.iv), iv);

	std::string plaintext;
	CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption decryptor;
	decryptor.SetKeyWithIV(key, sizeof(key), iv);
	CryptoPP::StringSource string_source_decrypt(
			ciphertext,
			true,
			new CryptoPP::StreamTransformationFilter(
					decryptor,
					new CryptoPP::StringSink(plaintext)
			)
	);
	return plaintext;
}


std::string Crypto::to_base64(const std::string& raw_binary)
{
	std::string base64_string{};
	CryptoPP::StringSource string_source_base64(
			raw_binary,
			true,
			new CryptoPP::Base64Encoder(
					new CryptoPP::StringSink(base64_string),
					false /* No line breaks. */
			)
	);
	return base64_string;
}

std::string Crypto::to_bin(const std::string& base64)
{
	std::string raw_binary{};
	CryptoPP::StringSource string_source_bin(
			base64,
			true,
			new CryptoPP::Base64Decoder(
					new CryptoPP::StringSink(raw_binary)
			)
	);
	return raw_binary;
}

Credentials Crypto::encrypt_creds(Credentials creds)
{
	std::string username_encrypted{Crypto::encrypt(creds.username_)};
	std::string username_encrypted_b64{Crypto::to_base64(username_encrypted)};
	std::string pw_hashed_encryp{Crypto::encrypt(creds.pw_hashed_)};
	std::string pw_hashed_encryp_b64{Crypto::to_base64(pw_hashed_encryp)};
	return Credentials(username_encrypted_b64, pw_hashed_encryp_b64);
}

std::string Crypto::decrypt_from_b64(std::string base_64)
{
	return Crypto::decrypt(Crypto::to_bin(base_64));
}
