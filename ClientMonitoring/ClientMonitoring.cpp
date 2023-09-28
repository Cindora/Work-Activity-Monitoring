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

	unsigned int result;

	result = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (result) {
		cout << "WSA Startup failed. Result: " << result << endl;
		return 1;
	}

	
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	result = getaddrinfo("localhost", "111", &hints, &addrResult);

	if (result) {
		cout << "Getaddrinfo failed. Result: " << result << endl;
		WSACleanup();
		return 1;
	}

}