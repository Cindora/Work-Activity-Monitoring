﻿#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <windows.h>
#include <winSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>

using namespace std;
namespace fs = filesystem;

struct Data {
	string domain;
	string machine;
	string ip;
	string user;
	string activity;
};

void parseString(const char* input, Data& data) {
	stringstream ss(input);
	ss >> data.user >> data.machine >> data.domain >> data.ip >> data.activity;
}

void Cleanup(SOCKET ConnectSocket, ADDRINFO* addrResult) {	// Cleaning up the socket and address info
	if (ConnectSocket != INVALID_SOCKET) {
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
	}
	if (addrResult != NULL) {
		freeaddrinfo(addrResult);
	}
	WSACleanup();
}

void SaveData(const Data& data) {	// Saving data to a file (Data/"user".data)
	CreateDirectory("Data", NULL);
	ofstream outputFile("Data/" + data.user + ".data");
	if (outputFile.is_open()) { 
		outputFile << data.user << " " << data.machine << " " << 
			data.domain << " " << data.ip << " " << data.activity << endl;
		outputFile.close();  
	} 
	else { 
		cout << "Unable to open file for writing." << endl; 
	}
} 

void ReadAndWriteData(const string& username, ofstream& outputFile) {	// Helper function for UpdateConnectedClients
	ifstream clientInfo(username);
	if (clientInfo.is_open()) {
		string dataStr;
		if (getline(clientInfo, dataStr)) {
			outputFile << dataStr << endl;
		}
		clientInfo.close();
	}
	else {
		cout << "Unable to open file " << username << endl;
	}
}

void UpdateConnectedClients(const string& dataFolder, const string& outputFilename) {	// Read client information and write it to СonnectedСlients 
	ofstream outputFile(dataFolder + "/" + outputFilename);
	if (!outputFile.is_open()) {
		cout << "Unable to open output file." << endl;
		return;
	}

	for (const auto& entry : fs::directory_iterator(dataFolder)) {
		if (entry.is_regular_file()) {
			ReadAndWriteData(entry.path().string(), outputFile);
		}
	}

	outputFile.close();
}

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
		Cleanup(ListenSocket, addrResult);
		return 1;
	}


	ListenSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol); // Creating socket

	if (ListenSocket == INVALID_SOCKET) {
		cout << "Socket creation failed." << endl;
		Cleanup(ListenSocket, addrResult);
		return 1;
	}


	result = bind(ListenSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen); // Bind the socket to our address

	if (result == SOCKET_ERROR) {
		cout << "Binding server failed." << endl;
		Cleanup(ListenSocket, addrResult);
		return 1;
	}

	result = listen(ListenSocket, SOMAXCONN); // Set socket listening mode

	if (result == SOCKET_ERROR) {
		cout << "Listening socket failed." << endl;
		Cleanup(ListenSocket, addrResult);
		return 1;
	}

	char buff[1024];
	Data data;

	while (true) {										 // Listening and processing requests by the server
		cout << endl << "Server is listening." << endl;

		ClientSocket = accept(ListenSocket, NULL, NULL); // Connecting a client socket to the server

		if (ClientSocket == INVALID_SOCKET) {
			cout << "Accepting socket failed." << endl;
			Cleanup(ListenSocket, addrResult);
			return 1;
		}

		// Recieving and sending message
		ZeroMemory(buff, 1024);

		result = recv(ClientSocket, buff, 1024, 0); // Recieve message

		if (result > 0) {}
		else if (result == 0) {
			cout << "Connection closing." << endl;
		}
		else {
			cout << "Recieving failed with error." << endl;
			Cleanup(ClientSocket, addrResult);
			return 1;
		}
		//

		result = shutdown(ClientSocket, SD_SEND); // Shut down the socket to send

		if (result == SOCKET_ERROR) {
			cout << "Shutdown client socket failed." << endl;
			Cleanup(ClientSocket, addrResult);
			return 1;
		}
		
		parseString(buff, data);
		SaveData(data);		// Saving data in a file

		UpdateConnectedClients("Data", "ConnectedClients.data");	// Update information about connected clients
	}	// Stop listening

	closesocket(ListenSocket);

	Cleanup(ClientSocket, addrResult);
	return 0;

}
