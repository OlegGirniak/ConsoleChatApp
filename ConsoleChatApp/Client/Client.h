#pragma once
#include <iostream>
#include <assert.h>
#include <vector>
#include <WinSock2.h>
#include <string>
#include <thread>
#include <mutex>
#include <queue>
#include <unordered_map>

#pragma warning(disable: 4996)

#pragma comment(lib, "ws2_32.lib")

#define SIZE (1024)

using std::cout, std::cin, std::endl, std::string, std::thread, std::mutex, std::vector, std::unordered_map, std::pair, std::move, std::ref, std::getline;

WSADATA wsa;

void WSAStart()
{
	assert(!(WSAStartup(MAKEWORD(2, 2), &wsa)) && "Couldn't init wsa");
	cout << "WSA startup +" << endl;
}

void CreateSocket(SOCKET& client_socket, struct sockaddr_in* server_addr)
{
	assert(!((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR) && ("Couldn't create socket"));
	cout << "Client socket created + " << endl;

	string ip("127.0.0.1");
	unsigned long ip_addr = inet_addr(ip.c_str());

	server_addr->sin_family = AF_INET;
	server_addr->sin_port = htons(2222);
	server_addr->sin_addr.s_addr = ip_addr;

	cout << "Set server info + " << endl;
}

void Send(SOCKET& client_socket, struct sockaddr_in* server_addr)
{
	string message;


	assert(!(connect(client_socket, (struct sockaddr*)server_addr, sizeof(*server_addr))));
	cout << "Connect to server + " << endl;

	while (message != "Exit")
	{
		getline(cin, message);

		assert((send(client_socket, message.c_str(), message.size(), 0)) != -1);

		cout << "Messages is sent" << endl;
	}
}

void Receive(SOCKET& client_socket)
{
	char buffer[SIZE]{};

	while (true)
	{
		int bytes_received = recv(client_socket, buffer, SIZE, 0);

		if (bytes_received > 0)
		{
			for (int i = 0; i < strlen(buffer); i++)
			{
				cout << buffer[i];
			}
			cout << endl;
		}

		memset(buffer, 0, SIZE);
	}
}

int main()
{
	WSAStart();

	SOCKET client_socket;

	sockaddr_in server_addr{};

	CreateSocket(client_socket, &server_addr);

	thread recv_thread = thread(Receive, ref(client_socket));

	thread send_thread = thread(Send, ref(client_socket), &server_addr);


	recv_thread.join();
	send_thread.join();
}