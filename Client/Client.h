#pragma once
#include "Raknet\RakPeerInterface.h"

class Client {
public:
	Client();
	~Client();

	void StartUp();
	void Update();

	void HandleNetworkConnection();
	void InitaiseClientConnection();

	void HandleNetworkMessages();

private:

	RakNet::RakPeerInterface* m_pPeerInterface;
};