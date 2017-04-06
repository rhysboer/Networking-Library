#pragma once
#include <iostream>
#include <cstdio>
#include <thread>
#include <string>
#include <vector>
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "GameMessages.h"

class Client {
public:
	Client();
	~Client();

	void StartUp(char* ip, unsigned short port);
	void Update();

private:
	// Initialize the connection
	void HandleNetworkConnection();
	void InitaiseClientConnection(char* ip, unsigned short port);

	// Handle incoming packets
	void HandleNetworkMessages();

	// Private Variables
	RakNet::RakPeerInterface* m_pPeerInterface;

	// CHAT

	std::thread* m_thread;

	void Test();

	void AddMessage(const std::string message);
	void PrintBuffer();
	void FillBuffer();

	bool m_hasBufferChanged = true;
	
	const short CHAT_BUFFER_SIZE = 22;
	std::string m_buffer = "";
	std::vector<std::string> m_chatBuffer;


	char* IP = "0.0.0.0";
	unsigned short PORT = 0;
};