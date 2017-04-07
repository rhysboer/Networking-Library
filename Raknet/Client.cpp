#include "Client.h"

Client::Client() {
}

Client::~Client() {
	m_inputThread->join();
	delete m_inputThread;
	m_inputThread = nullptr;
}


void Client::StartUp(char* ip, unsigned short port) {
	HandleNetworkConnection();
	InitaiseClientConnection(ip, port);

	FillBuffer();

	// Input Thread
	m_inputThread = new std::thread(&Client::GetInput, this);
	m_printThread = new std::thread(&Client::PrintBuffer, this);
}

void Client::Update() {
	HandleNetworkMessages();
}

void Client::SetUsername(const char * username) {
	m_username = username;
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
		case ID_SERVER_SET_CLIENT_ID:
			OnSetClientID(packet);
			break;
		case ID_CLIENT_CLIENT_DATA:
			OnReceivedClientDataPacket(packet);
			break;
		case ID_SERVER_TEXT_MESSAGE:
		{
			RakNet::BitStream bsIn(packet->data, packet->length, false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

			RakNet::RakString str;
			bsIn.Read(str);
			AddMessage(str.C_String());

			break;
		}
		default: 
			std::cout << "Received a message with a unknown id: " << packet->data[0];
			break; 
		}
	}
}

void Client::OnSetClientID(RakNet::Packet * packet) {
	RakNet::BitStream bsIn(packet->data, packet->length, false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
	bsIn.Read(m_clientID);

	std::string message = "Client ID = ";
	message += std::to_string(m_clientID);
	AddMessage(message);
}

void Client::OnReceivedClientDataPacket(RakNet::Packet * packet) {
	RakNet::BitStream bsIn(packet->data, packet->length, false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

	int clientID;
	bsIn.Read(clientID);

	if(clientID != m_clientID) {
		RakNet::RakString message;
		bsIn.Read(message);

		std::string finalMessage = message.C_String();
		AddMessage(finalMessage);
	}
}

void Client::GetInput() {
	while(true) {
		if(_kbhit()) {
			int ch = _getch();

			switch(ch) {
			case BACKSPACE:
				if(m_buffer.size() > 0) {
					m_buffer.pop_back();
					m_hasBufferChanged = true;
				}
				break;
			case ENTER:
				m_buffer = m_username + ": " + m_buffer;
				AddMessage(m_buffer);
				SendServerMessage(m_buffer.c_str());
				m_buffer.clear();
				break;
			default:
				m_buffer += ch;
				m_hasBufferChanged = true;
				break;
			}
		}
	}
}

void Client::SendServerMessage(const std::string message) {
	RakNet::BitStream bs;
	bs.Write((RakNet::MessageID)GameMessages::ID_CLIENT_CLIENT_DATA);
	bs.Write(m_clientID);

	RakNet::RakString sendMessage = message.c_str();
	bs.Write(sendMessage);

	m_pPeerInterface->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
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
	while(true) {
		if(m_hasBufferChanged) {
			system("cls");
			for(int i = CHAT_BUFFER_SIZE - 1; i >= 0; i--) {
				std::cout << m_chatBuffer[i] << std::endl;
			}

			std::cout << "____________________" << std::endl;
			std::cout << "> " << m_buffer;

			m_hasBufferChanged = false;
		}
	}
}

void Client::FillBuffer() {
	for(int i = 0; i < CHAT_BUFFER_SIZE; i++) {
		m_chatBuffer.push_back("");
	}
}