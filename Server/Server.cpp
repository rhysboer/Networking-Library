#include "Server.h"

Server::Server() {
}

Server::~Server() {
}

void Server::StartUp(unsigned short port) {
	CreateServer("My Server", port);
}

void Server::Update() {
	HandleNetworkMessages();
}


void Server::CreateServer(const char* serverName, unsigned short port) {
	m_port = port;
	m_pPeerInterface = RakNet::RakPeerInterface::GetInstance();

	std::cout << "Starting up the server..." << std::endl;

	RakNet::SocketDescriptor sd(port, 0);

	m_pPeerInterface->Startup(32, &sd, 1);
	m_pPeerInterface->SetMaximumIncomingConnections(32);
}

void Server::SendNewClientID(RakNet::RakPeerInterface * pPeerInterface, RakNet::SystemAddress & address) {
	RakNet::BitStream bs;
	bs.Write((RakNet::MessageID)GameMessages::ID_SERVER_SET_CLIENT_ID);
	bs.Write(m_nextClientID);
	++m_nextClientID;

	pPeerInterface->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, address, false);
}

void Server::HandleNetworkMessages() {
	RakNet::Packet* packet = nullptr;

	while (true) {
		for (packet = m_pPeerInterface->Receive(); packet; m_pPeerInterface->DeallocatePacket(packet), packet = m_pPeerInterface->Receive()) {
			switch (packet->data[0]) {
			case ID_NEW_INCOMING_CONNECTION:
				// Incoming Connection
				std::cout << "Incoming Connection..." << std::endl;
				SendNewClientID(m_pPeerInterface, packet->systemAddress);
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				// A client has disconnected
				std::cout << "A client has disconnected" << std::endl;
				break;
			case ID_CONNECTION_LOST:
				// A client lost connection
				std::cout << "A client lost connection" << std::endl;
				break;
			case ID_CLIENT_CLIENT_DATA:
			{
				std::cout << "Client has sent a message" << std::endl;

				RakNet::BitStream bs(packet->data, packet->length, false);
				m_pPeerInterface->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, true);
				
				break;
			}
			default:
				// Received a message with an unknown id
				std::cout << "Received a message with an unknown ID: " << packet->data[0] << std::endl;
				break;
			}
		}
	}
}