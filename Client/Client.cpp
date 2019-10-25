#include <stdio.h>
#include <winsock2.h>
#include <string>
#include <iostream>

#pragma warning(disable : 4996)
#pragma comment(lib, "WS2_32.lib")

using namespace std;

int main()
{
	setlocale(LC_ALL, "rus");

	WORD ver = MAKEWORD(2, 0);
	WSADATA wsaData;
	int retVal = 0;

	WSAStartup(ver, (LPWSADATA)&wsaData);

	LPHOSTENT hostEnt;

	hostEnt = gethostbyname("localhost");

	if (!hostEnt)
	{
		cout << "Unable to collect gethostbyname\n";
		WSACleanup();
		return 1;
	}

	//Создаем сокет
	SOCKET clientSock = socket(AF_INET, SOCK_STREAM, 0);

	if (clientSock == SOCKET_ERROR)
	{
		cout << "Unable to create socket\n";
		WSACleanup();
		return 1;
	}

	SOCKADDR_IN serverInfo;

	serverInfo.sin_family = AF_INET;
	serverInfo.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverInfo.sin_port = htons(2000);

	retVal = connect(clientSock, (LPSOCKADDR)&serverInfo, sizeof(serverInfo));
	if (retVal == SOCKET_ERROR)
	{
		cout << "Unable to connect\n";
		WSACleanup();
		return 1;
	}

	cout << "Connection made sucessfully\n";

	string message;
	cout << "Enter message for server: \n";
	cin >> message;

	retVal = send(clientSock, (message.append("\0")).c_str(), sizeof(message), 0);

	if (retVal == SOCKET_ERROR)
	{
		cout << "Unable to send\n";
		getchar();
		WSACleanup();
		return 1;
	}

	string response;
	retVal = recv(clientSock, (char*)response.c_str(), sizeof(response) , 0);

	if (retVal == SOCKET_ERROR)
	{
		cout << "Unable to recv\n";
		WSACleanup();
		return 1;
	}

	cout << "Got the response from server\n" << response << endl;
	getchar();
	closesocket(clientSock);
	WSACleanup();

	return 0;
}