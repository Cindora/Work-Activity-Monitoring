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


	//result = connect(ConnectSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen); // Connecting to the server

	//if (result == SOCKET_ERROR) {
	//	cout << "Connection to the server failed." << endl;
	//	closesocket(ConnectSocket);
	//	ConnectSocket = INVALID_SOCKET;
	//	freeaddrinfo(addrResult);
	//	WSACleanup();
	//	return 1;
	//}

	////   Client connected to the server   //

	//result = shutdown(ConnectSocket, SD_SEND); // Shut down the socket to send

	//if (result == SOCKET_ERROR) {
	//	closesocket(ConnectSocket);
	//	freeaddrinfo(addrResult);
	//	WSACleanup();
	//	return 1;
	//}

}