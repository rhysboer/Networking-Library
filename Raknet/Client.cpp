#include "Client.h"

Client::Client() {
}

Client::~Client() {
	m_thread->join();
	delete m_thread;
	m_thread = nullptr;
}


void Client::StartUp(char* ip, unsigned short port) {
	HandleNetworkConnection();
	InitaiseClientConnection(ip, port);

	FillBuffer();

	m_thread = new std::thread(&Client::Test, this);
}

void Client::Update() {
	HandleNetworkMessages();
	PrintBuffer();
}

void Client::HandleNetworkConnection() {
	m_pPeerInterface = RakNet::RakPeerInterface::GetInstance();
}

void Client::InitaiseClientConnection(char* ip, const unsigned short port) {
	RakNet::SocketDescriptor sd;
	
	IP = ip;
	PORT = port;

	m_pPeerInterface->Startup(1, &sd, 1);
	
	std::cout << "Connecting to server at: " << IP << std::endl;
	
	RakNet::ConnectionAttemptResult res = m_pPeerInterface->Connect(IP, PORT, nullptr, 0);
	
	if(res != RakNet::CONNECTION_ATTEMPT_STARTED) {
		std::cout << "Unable to start connection, Error number: " << (unsigned char)res << std::endl;
	}
}

void Client::HandleNetworkMessages() {
	RakNet::Packet* packet;
	
	for(packet = m_pPeerInterface->Receive(); packet; m_pPeerInterface->DeallocatePacket(packet), packet = m_pPeerInterface->Receive()) {
		switch(packet->data[0]) {
		case ID_REMOTE_DISCONNECTION_NOTIFICATION:
			std::cout << "Another client has disconnected.\n";
			break;
		case ID_REMOTE_CONNECTION_LOST:
			std::cout << "Another client has lost the connection.\n";
			break;
		case ID_REMOTE_NEW_INCOMING_CONNECTION:
			std::cout << "Another client has connected.\n";
			break;
		case ID_CONNECTION_REQUEST_ACCEPTED:
			//std::cout << "Successfully connected to server!.\n";
			AddMessage("Successfully connected to server!.");
			break;
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			std::cout << "The server is full.\n";
			break;
		case ID_DISCONNECTION_NOTIFICATION:
			std::cout << "We have been disconnected.\n";
			break;
		case ID_CONNECTION_LOST:
			std::cout << "Connection lost.\n";
			break;
		case ID_SERVER_TEXT_MESSAGE:
		{
			RakNet::BitStream bsIn(packet->data, packet->length, false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

			RakNet::RakString str;
			bsIn.Read(str);
			AddMessage(str.C_String());
			//std::cout << str.C_String() << std::endl;

			break;
		}
		default: 
			std::cout << "Received a message with a unknown id: " << packet->data[0];
			break; 
		}
	}
}

void Client::Test() {
	while(1) {
		COORD coord;
		coord.X = 0;
		coord.Y = 0;

		//if(m_hasBufferChanged) {
			for(int i = CHAT_BUFFER_SIZE - 1; i > 0; i--) {
				coord.X = 0;
				coord.Y = i;
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
				std::cout << m_chatBuffer[i] << std::endl;
			}

			m_hasBufferChanged = false;

			coord.X = m_buffer.size();
			coord.Y = CHAT_BUFFER_SIZE;
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
		//}
	}
}

void Client::AddMessage(const std::string message) {
	for(int i = CHAT_BUFFER_SIZE - 1; i >= 0; i--) {
		if(i == 0) {
			m_chatBuffer[0] = message;
		} else {
			m_chatBuffer[i] = m_chatBuffer[i - 1];
		}
	}

	m_hasBufferChanged = true;
}

void Client::PrintBuffer() {
	//if(m_hasBufferChanged) {
	//	for(int i = CHAT_BUFFER_SIZE - 1; i > 0; i--) {
	//		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	//		std::cout << m_chatBuffer[i] << std::endl;
	//	}
	//}

	std::cin >> m_buffer;
	AddMessage(m_buffer);
}

void Client::FillBuffer() {
	for(int i = 0; i < CHAT_BUFFER_SIZE; i++) {
		m_chatBuffer.push_back("");
	}
}


/*
temp = 4

5
4
3
2
1
1

*/