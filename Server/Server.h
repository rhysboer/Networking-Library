#pragma once
#include "Raknet\RakPeerInterface.h"
#include "Raknet\MessageIdentifiers.h"
#include "Raknet\BitStream.h"

class Server {
public:
	Server();
	~Server();

	void CreateServer(const char* serverName, unsigned short port);
	void JoinServer(const char* ip, int port, const char* name);

	void HandleNetworkMessages();

private:

	RakNet::RakPeerInterface* m_pPeerInterface;

	unsigned short m_port;
	bool m_running;
	char* m_serverName;
};

