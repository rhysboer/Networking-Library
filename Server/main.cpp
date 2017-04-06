#include <iostream>
#include "Server.h"

using namespace std;

void PingClient(RakNet::RakPeerInterface * pPeerInterface);

void main() {

	Server* server = new Server;
	server->StartUp(5456);

	thread pingThread(PingClient, server->m_pPeerInterface);

	while(1) {
		server->Update();
	}
	

	delete server;
	server = nullptr;

	return;
}

void PingClient(RakNet::RakPeerInterface * pPeerInterface) {
	int i = 0;
	while(true) {
		std::string ping = "ping ";
		ping.append(std::to_string(i));

		RakNet::BitStream bs;
		bs.Write((RakNet::MessageID)GameMessages::ID_SERVER_TEXT_MESSAGE);
		bs.Write(ping.c_str());

		pPeerInterface->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
		std::this_thread::sleep_for(std::chrono::seconds(5));
		i++;
	}
}