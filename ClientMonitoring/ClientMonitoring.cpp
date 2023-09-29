#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <windows.h>
#include <winSock2.h>
#include <WS2tcpip.h>


using namespace std;

void Cleanup(SOCKET ConnectSocket, ADDRINFO* addrResult) {
	if (ConnectSocket != INVALID_SOCKET) {
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
	}
		

	if (addrResult != NULL) {
		freeaddrinfo(addrResult);
	}
		
	WSACleanup();
}

int ClientMonitoring()
{
	WSADATA wsaData;
	ADDRINFO hints;
	ADDRINFO* addrResult = NULL;
	SOCKET ConnectSocket = INVALID_SOCKET;

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

	result = getaddrinfo("localhost", "111", &hints, &addrResult); // Creating a path to the server
	if (result) {
		cout << "Path creation failed. Result: " << result << endl;
		Cleanup(ConnectSocket, addrResult);
		return 1;
	}


	ConnectSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol); // Creating socket

	if (ConnectSocket == INVALID_SOCKET) {
		cout << "Socket creation failed." << endl;
		Cleanup(ConnectSocket, addrResult);
		return 1;
	}


	result = connect(ConnectSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen); // Connecting to the server

	if (result == SOCKET_ERROR) {
		cout << "Connection to the server failed." << endl;
		Cleanup(ConnectSocket, addrResult);
		return 1;
	}

	///   Client connected to the server   ///

	// Sending message

	const char* messageToSend = "Client to Server message.";

	result = send(ConnectSocket, messageToSend, (int)strlen(messageToSend), 0); // Sending message

	if (result == SOCKET_ERROR) {
		cout << "Message send failed. Result: " << result << endl;
		Cleanup(ConnectSocket, addrResult);
		return 1;
	}

	//


	result = shutdown(ConnectSocket, SD_SEND); // Shut down the socket to send

	if (result == SOCKET_ERROR) {
		Cleanup(ConnectSocket, addrResult);
		return 1;
	}


	// Recieving message

	char buff[256];
	ZeroMemory(buff, 256);

	do {
		result = recv(ConnectSocket, buff, 256, 0); // Recieving message

		if (result > 0) {
			cout << "Recieved message: " << buff << endl;
		}
		else if (result == 0) {
			cout << "Connection closed." << endl;
		}
		else {
			cout << "Recieving failed with error." << endl;
		}
	} while (result > 0);

	//


	Cleanup(ConnectSocket, addrResult);
	return 0;
}

int main()
{
	ClientMonitoring();
	ClientMonitoring();
}