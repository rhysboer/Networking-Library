#include <iostream>
#include <conio.h>
#include <thread>
#include <string>

#include "Client.h"

void SetUsername(std::string& username);

void main() {
	Client* client = new Client();
	std::string username;

	SetUsername(username);
	client->StartUp("127.0.0.1", 5456);
	client->SetUsername(username.c_str());

	while (1) {
		client->Update();
	}

	system("pause");
	return;
}

void SetUsername(std::string& username) {
	std::cout << "Username: ";

	char tempName[16];
	while(true) {
		std::cin.getline(tempName, 16);

		if(!std::cin.fail()) {
			username = tempName;
			return;
		}
		std::cin.clear();
	}
}
