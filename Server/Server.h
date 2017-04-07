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

	// Test
	

	void HandleNetworkMessages();

	RakNet::RakPeerInterface* m_pPeerInterface;
private:
	void CreateServer(const char* serverName, unsigned short port);
	void SendNewClientID(RakNet::RakPeerInterface* pPeerInterface, RakNet::SystemAddress& address);

	unsigned int m_nextClientID = 1;
	unsigned short m_port;
};

