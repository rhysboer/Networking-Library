#pragma once
#include <iostream>
#include <conio.h>
#include <thread>
#include <string>
#include <vector>
#include <mutex>
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "GameMessages.h"

#define ENTER 13
#define BACKSPACE 8

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

	std::thread* m_inputThread;
	std::thread* m_printThread;

	std::mutex mutex;

	void GetInput();

	void AddMessage(const std::string message);
	void PrintBuffer();
	void FillBuffer();

	bool m_hasBufferChanged = true;
	
	const short CHAT_BUFFER_SIZE = 12;
	std::string m_buffer = "";
	std::vector<std::string> m_chatBuffer;


	char* IP = "0.0.0.0";
	unsigned short PORT = 0;
};