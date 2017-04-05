#include "Server.h"

Server::Server() {
	m_running = false;
}

Server::~Server() {
}


void Server::CreateServer(const char* serverName, unsigned short port) {
	m_port = port;
	m_pPeerInterface = RakNet::RakPeerInterface::GetInstance();

	RakNet::SocketDescriptor sd(port, 0);

	m_pPeerInterface->Startup(32, &sd, 1);
	m_pPeerInterface->SetMaximumIncomingConnections(32);

	m_running = true;
}

void Server::HandleNetworkMessages() {
	RakNet::Packet* packet = nullptr;

	while (true) {
		for (packet = m_pPeerInterface->Receive(); packet; m_pPeerInterface->DeallocatePacket(packet), packet = m_pPeerInterface->Receive()) {
			switch (packet->data[0]) {
			case ID_NEW_INCOMING_CONNECTION:
				// Incoming Connection
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				// A client has disconnected
				break;
			case ID_CONNECTION_LOST:
				// A client lost connection
				break;
			default:
				// Received a message with an unknown id
				packet->data[0];
				break;
			}
		}
	}
}