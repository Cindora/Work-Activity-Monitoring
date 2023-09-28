#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <windows.h>
#include <winSock2.h>
#include <WS2tcpip.h>


using namespace std;

int main()
{
	WSADATA wsaData;
	ADDRINFO hints;
	ADDRINFO* addrResult = NULL;
	SOCKET ClientSocket = INVALID_SOCKET;
	SOCKET ListenSocket = INVALID_SOCKET;

	unsigned int result;

	result = WSAStartup(MAKEWORD(2, 2), &wsaData); // Initiate use of the Win socket 

	if (result) {
		cout << "WSA Startup failed. Result: " << result << endl;
		return 1;
	}


	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	result = getaddrinfo(NULL, "111", &hints, &addrResult); // Creating a path to the server
	if (result) {
		cout << "Path creation failed. Result: " << result << endl;
		WSACleanup();
		return 1;
	}


	ListenSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol); // Creating socket

	if (ListenSocket == INVALID_SOCKET) {
		cout << "Socket creation failed." << endl;
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}


	result = bind(ListenSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen); // Bind the socket to our address

	if (result == SOCKET_ERROR) {
		cout << "Binding server failed." << endl;
		closesocket(ListenSocket);
		ListenSocket = INVALID_SOCKET;
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}

	result = listen(ListenSocket, SOMAXCONN); // Set socket listening mode

	if (result == SOCKET_ERROR) {
		cout << "Listening socket failed." << endl;
		closesocket(ListenSocket);
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}

	ClientSocket = accept(ListenSocket, NULL, NULL); // Connecting a client socket to the server

	if (ClientSocket == INVALID_SOCKET) {
		cout << "Accepting socket failed." << endl;
		closesocket(ListenSocket);
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}

	closesocket(ListenSocket);

	///   Client connected to the server   ///


	// Recieving and sending message

	char buff[256];
	ZeroMemory(buff, 256);

	const char* messageToSend = "Server to Client message.";

	do {
		result = recv(ClientSocket, buff, 256, 0); // Recieving message

		if (result > 0) {
			cout << "Recieved message: " << buff << endl;

			result = send(ClientSocket, messageToSend, (int)strlen(messageToSend), 0); // Sending message

			if (result == SOCKET_ERROR) {
				cout << "Message send failed. Result: " << result << endl;
				closesocket(ClientSocket);
				freeaddrinfo(addrResult);
				WSACleanup();
				return 1;
			}
		}
		else if (result == 0) {
			cout << "Connection closing." << endl;
		}
		else {
			cout << "Recieving failed with error." << endl;
			closesocket(ClientSocket);
			freeaddrinfo(addrResult);
			WSACleanup();
			return 1;
		}
	} while (result > 0);

	//


	result = shutdown(ClientSocket, SD_SEND); // Shut down the socket to send

	if (result == SOCKET_ERROR) {
		cout << "Shutdown client socket failed." << endl;
		closesocket(ClientSocket);
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}

	closesocket(ClientSocket);
	freeaddrinfo(addrResult);
	WSACleanup();
	return 0;

}