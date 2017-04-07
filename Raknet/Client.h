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

	void SetUsername(const char* username);

private:
	// Initialize the connection
	void HandleNetworkConnection();
	void InitaiseClientConnection(char* ip, unsigned short port);

	// Handle incoming packets
	void HandleNetworkMessages();

	void OnSetClientID(RakNet::Packet* packet);
	void OnReceivedClientDataPacket(RakNet::Packet* packet);

	// CHAT
	std::thread* m_inputThread;
	std::thread* m_printThread;
	void GetInput();

	void SendServerMessage(const std::string message);
	void AddMessage(const std::string message);
	void PrintBuffer();
	void FillBuffer();

	bool m_hasBufferChanged = true;
	
	const short CHAT_BUFFER_SIZE = 12;
	std::string m_buffer = "";
	std::vector<std::string> m_chatBuffer;

	// Variables
	RakNet::RakPeerInterface* m_pPeerInterface;

	char* IP = "0.0.0.0";
	unsigned short PORT = 0;
	unsigned int m_clientID = 0;
	std::string m_username = "NoName";
};