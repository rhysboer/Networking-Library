#include <iostream>
#include <conio.h>
#include <thread>
#include <string>

#include "Client.h"

void main() {

	Client* client = new Client();
	client->StartUp("127.0.0.1", 5456);
	

	while (1) {
		client->Update();
	}

	system("pause");
	return;
}