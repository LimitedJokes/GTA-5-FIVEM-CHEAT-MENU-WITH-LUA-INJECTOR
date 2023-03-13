#pragma once
#include <cryptopp/aes.h>
#include <cryptopp/osrng.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/base64.h>
#include <cryptopp/filters.h>
#include <cryptopp/modes.h>
#include <cryptopp/hex.h>
#include <cryptopp/ccm.h>

#include <atlsecurity.h> 
#include <strsafe.h> 
#include <windows.h>
#include <string>
#include <stdio.h>
#include <iostream>

#include <curl/curl.h>

#include <shellapi.h>

#include <sstream> 
#include <iomanip> 
#include "../xorstr.hpp"
#include <fstream> 

#include <nlohmann/json.hpp>

#pragma comment(lib, "rpcrt4.lib")

namespace KeyAuth {

	class encryption {
	public:
		std::string name;
		static std::string encrypt_string(const std::string& plain_text, const std::string& key, const std::string& iv) {
			std::string cipher_text;

			try {
				CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption encryption;
				encryption.SetKeyWithIV((CryptoPP::byte*)key.c_str(), key.size(), (CryptoPP::byte*)iv.c_str());

				CryptoPP::StringSource encryptor(plain_text, true,
					new CryptoPP::StreamTransformationFilter(encryption,
						new CryptoPP::HexEncoder(
							new CryptoPP::StringSink(cipher_text),
							false
						)
					)
				);
			}
			catch (CryptoPP::Exception& ex) {
				system(XorStr("cls").c_str());
				std::cout << ex.what();
				exit(0);
			}
			return cipher_text;
		}

		static std::string decrypt_string(const std::string& cipher_text, const std::string& key, const std::string& iv) {
			std::string plain_text;

			try {
				CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption decryption;
				decryption.SetKeyWithIV((CryptoPP::byte*)key.c_str(), key.size(), (CryptoPP::byte*)iv.c_str());

				CryptoPP::StringSource decryptor(cipher_text, true,
					new CryptoPP::HexDecoder(
						new CryptoPP::StreamTransformationFilter(decryption,
							new CryptoPP::StringSink(plain_text)
						)
					)
				);
			}
			catch (CryptoPP::Exception& ex) {
				system(XorStr("cls").c_str());
				std::cout << ex.what();
				exit(0);
			}
			return plain_text;
		}

		static std::string sha256(const std::string& plain_text) {
			std::string hashed_text;
			CryptoPP::SHA256 hash;

			try {
				CryptoPP::StringSource hashing(plain_text, true,
					new CryptoPP::HashFilter(hash,
						new CryptoPP::HexEncoder(
							new CryptoPP::StringSink(hashed_text),
							false
						)
					)
				);
			}
			catch (CryptoPP::Exception& ex) {
				system(XorStr("cls").c_str());
				std::cout << ex.what();
				exit(0);
			}

			return hashed_text;
		}

		static std::string encode(const std::string& plain_text) {
			std::string encoded_text;

			try {
				CryptoPP::StringSource encoding(plain_text, true,
					new CryptoPP::HexEncoder(
						new CryptoPP::StringSink(encoded_text),
						false
					)
				);
			}
			catch (CryptoPP::Exception& ex) {
				system(XorStr("cls").c_str());
				std::cout << ex.what();
				exit(0);
			}

			return encoded_text;
		}

		static std::string decode(const std::string& encoded_text) {
			std::string out;

			try {
				CryptoPP::StringSource decoding(encoded_text, true,
					new CryptoPP::HexDecoder(
						new CryptoPP::StringSink(out)
					)
				);
			}
			catch (CryptoPP::Exception& ex) {
				system(XorStr("cls").c_str());
				std::cout << ex.what();
				exit(0);
			}

			return out;
		}

		static std::string iv_key() {
			UUID uuid = { 0 };
			std::string guid;

			::UuidCreate(&uuid);

			RPC_CSTR szUuid = NULL;
			if (::UuidToStringA(&uuid, &szUuid) == RPC_S_OK)
			{
				guid = (char*)szUuid;
				::RpcStringFreeA(&szUuid);
			}

			return guid.substr(0, 16);
		}

		static std::string encrypt(std::string message, std::string enc_key, std::string iv) {
			enc_key = sha256(enc_key).substr(0, 32);
			iv = sha256(iv).substr(0, 16);
			return encrypt_string(message, enc_key, iv);
		}

		static std::string decrypt(std::string message, std::string enc_key, std::string iv) {
			enc_key = sha256(enc_key).substr(0, 32);

			iv = sha256(iv).substr(0, 16);

			return decrypt_string(message, enc_key, iv);
		}
	};

	class utils {
	public:

		static std::string get_hwid() {
			ATL::CAccessToken accessToken;
			ATL::CSid currentUserSid;
			if (accessToken.GetProcessToken(TOKEN_READ | TOKEN_QUERY) &&
				accessToken.GetUser(&currentUserSid))
				return std::string(CT2A(currentUserSid.Sid()));
		}

		static std::time_t string_to_timet(std::string timestamp) {
			auto cv = strtol(timestamp.c_str(), NULL, 10);

			return (time_t)cv;
		}

		static std::tm timet_to_tm(time_t timestamp) {
			std::tm context;

			localtime_s(&context, &timestamp);

			return context;
		}

	};


	auto iv = encryption::sha256(encryption::iv_key());
	class api {


	public:

		std::string name, ownerid, secret, version;

		api(std::string name, std::string ownerid, std::string secret, std::string version)
			: name(name), ownerid(ownerid), secret(secret), version(version) {}

		void init()
		{
			enckey = encryption::sha256(encryption::iv_key());
			if (ownerid.length() != 10 || secret.length() != 64)
			{
				std::cout << XorStr("\n\n Application Not Setup Correctly. Please Watch Video Linked in Main.cpp");
				Sleep(4500);
				exit(0);
			}

			auto data =
				XorStr("type=").c_str() + encryption::encode(XorStr("init").c_str()) +
				XorStr("&ver=").c_str() + encryption::encrypt(version, secret, iv) +
				XorStr("&enckey=").c_str() + encryption::encrypt(enckey, secret, iv) +
				XorStr("&name=").c_str() + encryption::encode(name) +
				XorStr("&ownerid=").c_str() + encryption::encode(ownerid) +
				XorStr("&init_iv=").c_str() + iv;

			auto response = req(data);

			response = encryption::decrypt(response, secret, iv);
			auto json = response_decoder.parse(response);

			if (json[("success")])
			{
				sessionid = json[("sessionid")];
				// optional success message
			}
			else if (json[("message")] == "invalidver")
			{
				std::string dl = json[("download")];
				ShellExecuteA(0, "open", dl.c_str(), 0, 0, SW_SHOWNORMAL);
				exit(0);
			}
			else
			{
				std::cout << "\n\n ";
				std::cout << std::string(json[("message")]);
				Sleep(4500);
				exit(0);
			}
		}

		void login(std::string username, std::string password)
		{
			printf("Login Start\n");
			std::string hwid = utils::get_hwid();
			auto iv = encryption::sha256(encryption::iv_key());
			auto data =
				XorStr("type=").c_str() + encryption::encode("login") +
				XorStr("&username=").c_str() + encryption::encrypt(username, enckey, iv) +
				XorStr("&pass=").c_str() + encryption::encrypt(password, enckey, iv) +
				XorStr("&hwid=").c_str() + encryption::encrypt(hwid, enckey, iv) +
				XorStr("&sessionid=").c_str() + encryption::encode(sessionid) +
				XorStr("&name=").c_str() + encryption::encode(name) +
				XorStr("&ownerid=").c_str() + encryption::encode(ownerid) +
				XorStr("&init_iv=").c_str() + iv;
			auto response = req(data);
			response = encryption::decrypt(response, enckey, iv);
			auto json = response_decoder.parse(response);

			if (json[("success")])
			{
				// optional success message
				load_user_data(json[("info")]);
//				printf("Seccuyed");
			}
			else
			{
				MessageBox(NULL, TEXT("Wrong Username or Password!!"), TEXT("Error"), MB_OK);
//				std::cout << XorStr("\n\n Status: Failure: ");
//				std::cout << std::string(json[("message")]);
//				Sleep(3500);
				exit(0);
			}
		}

void regstr(std::string username, std::string password, std::string key) {
    std::string hwid = utils::get_hwid();
    auto iv = encryption::gen_random_iv(); // Use a cryptographically secure method for generating the IV
    auto data =
        "type=register" +
        "&username=" + encryption::encrypt(username, enckey, iv) +
        "&pass=" + encryption::encrypt(password, enckey, iv) +
        "&key=" + encryption::encrypt(key, enckey, iv) +
        "&hwid=" + encryption::encrypt(hwid, enckey, iv) +
        "&sessionid=" + encryption::encode(sessionid) +
        "&name=" + encryption::encode(name) +
        "&ownerid=" + encryption::encode(ownerid) +
        "&init_iv=" + iv;
    auto response = req(data);
    response = encryption::decrypt(response, enckey, iv);
    auto json = response_decoder.parse(response);

    if (json["success"]) {
        // optional success message
        load_user_data(json["info"]);
    } else {
        MessageBox(NULL, TEXT("Regst Error!"), TEXT("Regst"), MB_OK);
        exit(0);
    }
}


		void upgrade(std::string username, std::string key) {

			auto iv = encryption::sha256(encryption::iv_key());
			auto data =
				XorStr("type=").c_str() + encryption::encode("upgrade") +
				XorStr("&username=").c_str() + encryption::encrypt(username, enckey, iv) +
				XorStr("&key=").c_str() + encryption::encrypt(key, enckey, iv) +
				XorStr("&sessionid=").c_str() + encryption::encode(sessionid) +
				XorStr("&name=").c_str() + encryption::encode(name) +
				XorStr("&ownerid=").c_str() + encryption::encode(ownerid) +
				XorStr("&init_iv=").c_str() + iv;
			auto response = req(data);
			response = encryption::decrypt(response, enckey, iv);
			auto json = response_decoder.parse(response);

			if (json[("success")])
			{
				// optional success message
			}
			else
			{
				std::cout << XorStr("\n\n Status: Failure: ");
				std::cout << std::string(json[("message")]);
				Sleep(3500);
				exit(0);
			}
		}

void license(const std::string& user_key) {
    // Get hardware ID of the machine
    std::string hardware_id;
    try {
        hardware_id = utils::get_hwid();
    } catch (const std::runtime_error& e) {
        throw std::runtime_error("Failed to retrieve hardware ID: " + std::string(e.what()));
    }

    // Generate random nonce for encryption
    const std::string nonce = encryption::random_nonce();

    // Create JSON object to send to server
    std::unique_ptr<json> data = std::make_unique<json>();
    data->emplace("type", "license");
    data->emplace("key", encryption::encrypt(user_key, enckey, nonce));
    data->emplace("hwid", encryption::encrypt(hardware_id, enckey, nonce));
    data->emplace("sessionid", sessionid);
    data->emplace("name", name);
    data->emplace("ownerid", ownerid);

    // Send data to server and get response
    std::string response;
    try {
        response = req(data.dump());
    } catch (const std::exception& e) {
        std::cerr << "Error: Failed to send data to server: " << e.what() << '\n';
        return;
    }

    // Parse response as JSON object and check if request was successful
    json json_response;
    try {
        json_response = json::parse(response);
    } catch (const std::exception& e) {
        std::cerr << "Error: Failed to parse response: " << e.what() << '\n';
        return;
    }

    if (json_response.value("success", false)) {
        try {
            const std::string decrypted_info = encryption::decrypt(json_response["info"], enckey, nonce);
            load_user_data(decrypted_info);
        } catch (const std::exception& e) {
            std::cerr << "Error: Failed to load user data: " << e.what() << '\n';
        }
    } else {
        std::cerr << "Error: " << json_response.value("message", "Unknown error") << '\n';
    }
}
	
		
void log(std::string message) {

			auto iv = encryption::sha256(encryption::iv_key());

			char acUserName[100];
			DWORD nUserName = sizeof(acUserName);
			GetUserNameA(acUserName, &nUserName);
			std::string UsernamePC = acUserName;

			auto data =
				XorStr("type=").c_str() + encryption::encode(XorStr("log").c_str()) +
				XorStr("&pcuser=").c_str() + encryption::encrypt(UsernamePC, enckey, iv) +
				XorStr("&message=").c_str() + encryption::encrypt(message, enckey, iv) +
				XorStr("&sessionid=").c_str() + encryption::encode(sessionid) +
				XorStr("&name=").c_str() + encryption::encode(name) +
				XorStr("&ownerid=").c_str() + encryption::encode(ownerid) +
				XorStr("&init_iv=").c_str() + iv;

			req(data);
		}

		std::vector<unsigned char> download(std::string fileid) {
			auto iv = encryption::sha256(encryption::iv_key());
			auto to_uc_vector = [](std::string value) {
				return std::vector<unsigned char>(value.data(), value.data() + value.length() + 1);
			};

			auto data =
				XorStr("type=").c_str() + encryption::encode("file") +
				XorStr("&fileid=").c_str() + encryption::encrypt(fileid, enckey, iv) +
				XorStr("&sessionid=").c_str() + encryption::encode(sessionid) +
				XorStr("&name=").c_str() + encryption::encode(name) +
				XorStr("&ownerid=").c_str() + encryption::encode(ownerid) +
				XorStr("&init_iv=").c_str() + iv;

			auto response = req(data);
			response = encryption::decrypt(response, enckey, iv);
			auto json = response_decoder.parse(response);

			if (!json["success"])
			{
				std::cout << XorStr("\n\n Status: Failure: ");
				std::cout << std::string(json[("message")]);
			}

			auto file = encryption::decode(json["contents"]);

			return to_uc_vector(file);
		}

void webhook(std::string id, std::string params) {

			auto iv = encryption::sha256(encryption::iv_key());

			auto data =
				XorStr("type=").c_str() + encryption::encode(XorStr("webhook").c_str()) +
				XorStr("&webid=").c_str() + encryption::encrypt(id, enckey, iv) +
				XorStr("&params=").c_str() + encryption::encrypt(params, enckey, iv) +
				XorStr("&sessionid=").c_str() + encryption::encode(sessionid) +
				XorStr("&name=").c_str() + encryption::encode(name) +
				XorStr("&ownerid=").c_str() + encryption::encode(ownerid) +
				XorStr("&init_iv=").c_str() + iv;

			auto response = req(data);
			
			response = encryption::decrypt(response, enckey, iv);
			std::cout << response;
			

			auto json = response_decoder.parse(response);

			if (json[("success")])
			{
				// optional success message
			}
			else
			{
				std::cout << XorStr("\n\n Status: Failure: ");
				std::cout << std::string(json[("message")]);
			}
		}


		class user_data_class {
		public:
			std::string username;
			std::tm expiry;
			std::string subscription;
		};

		user_data_class user_data;

private:
		std::string sessionid, enckey;

		static size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
			((std::string*)userp)->append((char*)contents, size * nmemb);
			return size * nmemb;
		}

		static std::string req(std::string data) {
			CURL* curl = curl_easy_init();

			if (!curl)
				return "null";

			std::string to_return;

			curl_easy_setopt(curl, CURLOPT_URL, XorStr("https://keyauth.uk/api/1.0/").c_str());

			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);

			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());

			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &to_return);

			auto code = curl_easy_perform(curl);

			if (code != CURLE_OK)
				MessageBoxA(0, curl_easy_strerror(code), 0, MB_ICONERROR);
			
			curl_easy_cleanup(curl);
			
			long http_code = 0;
			curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

			if (http_code == 429) // client was rate limited
			{
				std::cout << XorStr("\n\n You're connecting too fast to loader, slow down.");
				Sleep(3500);
				exit(0);
			}
			
			return to_return;
		}

		class user_data_structure {
		public:
			std::string username;
			std::string expiry;
			std::string subscription;
		};

void load_user_data(const nlohmann::json& data) {
    try {
        // Check if the "username" key is present and has a valid data type
        const auto& username = data.at("username").get<std::string>();

        // Check if the "subscriptions" key is present and has a valid data type
        const auto& subscriptions = data.at("subscriptions");
        if (subscriptions.empty() || !subscriptions.is_array()) {
            throw std::runtime_error("Error loading user data: missing or invalid 'subscriptions' key");
        }

        // Extract and validate the first subscription object from the "subscriptions" array
        const auto& first_subscription = subscriptions.at(0);
        const auto& expiry = first_subscription.at("expiry").get<std::string>();
        const auto& subscription = first_subscription.at("subscription").get<std::string>();
        if (expiry.empty() || subscription.empty()) {
            throw std::runtime_error("Error loading user data: missing or empty subscription data");
        }

        // Store the user data
        user_data.username = username;
        user_data.expiry = utils::string_to_timet(expiry);
        user_data.subscription = subscription;
    } catch (const std::exception& e) {
        // Handle errors here, such as logging or throwing a custom exception
        throw std::runtime_error("Error loading user data: " + std::string(e.what()));
    }
}

