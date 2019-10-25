#include <winsock2.h>
#include <stdio.h>
#include <string>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

int main(void)
{
	setlocale(LC_ALL, "rus");

	WORD sockVer;
	WSADATA wsaData;
	int retVal;

	sockVer = MAKEWORD(2, 0);

	WSAStartup(sockVer, &wsaData);

	//Создаем сокет
	SOCKET servSock = socket(AF_INET, SOCK_STREAM, 0);

	if (servSock == INVALID_SOCKET)
	{
		cout << "Unable to create socket\n";
		getchar();
		WSACleanup();
		return SOCKET_ERROR;
	}
	SOCKADDR_IN sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(2000);
	sin.sin_addr.s_addr = INADDR_ANY;

	retVal = bind(servSock, (LPSOCKADDR)&sin, sizeof(sin));
	if (retVal == SOCKET_ERROR)
	{
		cout << "Unable to bind\n";
		WSACleanup();
		return SOCKET_ERROR;
	}

	//Пытаемся начать слушать сокет
	retVal = listen(servSock, 10);

	if (retVal == SOCKET_ERROR)
	{
		cout << "Unable to listen\n";
		WSACleanup();
		return SOCKET_ERROR;
	}

	cout << "Waiting client.... \n";

	//Ждем клиента
	SOCKET clientSock;
	SOCKADDR_IN clnt;
	memset(&clnt, 0, sizeof(clnt));
	int lclnt = sizeof(clnt);

	clientSock = accept(servSock, (sockaddr*)&clnt, &lclnt);

	if (clientSock == INVALID_SOCKET)
	{
		cout << "Unable to accept\n";
		WSACleanup();
		return SOCKET_ERROR;
	}

	cout << "Client is connected.... \n";

	string response;
	retVal = recv(clientSock, (char*)response.c_str(), sizeof(response), 0);

	if (retVal == SOCKET_ERROR)
	{
		cout << "Unable to recv\n";
		return SOCKET_ERROR;
	}

	cout << "Got the response from client\n" << response << endl;
	getchar();

	string message;
	cout << "Enter message for client: \n";
	cin >> message;

	retVal = send(clientSock, message.append("\0").c_str(), sizeof(message), 0);

	if (retVal == SOCKET_ERROR)
	{
		cout << "Unable to send\n";
		return SOCKET_ERROR;
	}

	//Закрываем сокет
	closesocket(clientSock);
	closesocket(servSock);

	WSACleanup();
	return 0;
}
