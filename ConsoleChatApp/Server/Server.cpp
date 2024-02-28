#include <iostream>
#include <assert.h>
#include <vector>
#include <WinSock2.h>
#include <string>
#include <thread>
#include <mutex>
#include <unordered_map>

#pragma comment(lib, "ws2_32.lib")


#define PORT (2222)


using std::cout, std::endl, std::string, std::thread, std::mutex,
      std::vector, std::pair, std::move, std::ref, std::lock_guard, 
      std::lock, std::make_pair;


WSADATA wsa;

vector<thread> thread_pool;
mutex client_sockets_mutex;
vector<pair<SOCKET, sockaddr_in>> client_sockets;

void WSAStart()
{
    assert(!(WSAStartup(MAKEWORD(2, 2), &wsa)) && "Couldn't init wsa");
    cout << "WSA startup +" << endl;
}

void ServerListen(SOCKET& server_socket, struct sockaddr_in* server_addr)
{
    memset(server_addr, (char)0, sizeof(sockaddr_in));

    assert(!((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR) && ("Couldn't create socket"));
    cout << "Server socket created +" << endl;

    server_addr->sin_family = AF_INET;
    server_addr->sin_addr.s_addr = INADDR_ANY;
    server_addr->sin_port = htons(PORT);

    assert(!(bind(server_socket, (struct sockaddr*)server_addr, sizeof(sockaddr_in))) && "Couldn't bind socket");
    cout << "Server socket binded +" << endl;

    assert(!(listen(server_socket, 10)) && "Couldn't listen socket");
    cout << "Server socket listen + " << endl;
}

void ServerProccess(SOCKET client_socket, sockaddr_in client_addr)
{
    char buffer[1024]{};
    int buffer_size = 1024;

    cout << "Client socket info: ip(" << client_addr.sin_family << "), port(" << client_addr.sin_port << ")" << endl;

    cout << "Current thread id: " << std::this_thread::get_id() << endl;

    while (true)
    {
        int bytes_received = recv(client_socket, buffer, buffer_size, 0);

        if (bytes_received > 0)
        {
            for (int i = 0; i < bytes_received; i++)
            {
                cout << buffer[i];
            }
            cout << endl;

            // send all clients
            for (auto& client : client_sockets)
            {
                if (client.first != client_socket)
                {
                    send(client.first, buffer, buffer_size, 0);
                }
            }

        }
        else 
        {
            cout << "Client disconnected" << endl;
            break;
        }

        memset(buffer, 0, buffer_size);
    }

    closesocket(client_socket);
    {
        lock_guard<mutex> lock(client_sockets_mutex);
        for (auto it = client_sockets.begin(); it != client_sockets.end(); ++it)
        {
            if (it->first == client_socket)
            {
                client_sockets.erase(it);
                break;
            }
        }
    }
}

void AcceptNewClient(SOCKET server_socket, struct sockaddr_in* server_addr)
{
    struct sockaddr_in client_addr;
    int size_client_addr = sizeof(sockaddr_in);
    SOCKET client_socket;

    while (true)
    {
        assert((client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &size_client_addr)) != INVALID_SOCKET);
        cout << "New socket connection | + |" << endl;

        {
            lock_guard<mutex> lock(client_sockets_mutex);
            client_sockets.push_back(make_pair(client_socket, client_addr));
        }

        thread_pool.push_back(move(thread(ServerProccess, client_socket, client_addr)));

        cout << "count conn: " << client_sockets.size() << endl;
    }
}

int main()
{
    WSAStart();

    SOCKET server_socket;
    struct sockaddr_in server_addr;

    ServerListen(server_socket, &server_addr);

    AcceptNewClient(server_socket, &server_addr);

    return 0;
}
