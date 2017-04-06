#pragma once
#include <iostream>
#include <string>
#include <thread>

#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "GameMessages.h"

class Server {
public:
	Server();
	~Server();

	void StartUp(unsigned short port);
	void Update();

	void CreateServer(const char* serverName, unsigned short port);

	void HandleNetworkMessages();

	RakNet::RakPeerInterface* m_pPeerInterface;
private:


	unsigned short m_port;
	bool m_running;
	char* m_serverName;
};

