#include <iostream>
#include <assert.h>
#include <vector>
#include <WinSock2.h>
#include <string>
#include <thread>
#include <mutex>

#pragma comment(lib, "ws2_32.lib")

#pragma warning(disable: 4996)

#define SIZE (1024)

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::thread;
using std::mutex;
using std::vector;
using std::pair;
using std::move;
using std::ref;
using std::getline;


enum class UserChoice
{
	SignUp = 1,
	SignIn,
	StartChating,
	Exit
};

class User
{
    static User* instance;

    string name;
    string password;

    WSADATA wsa;

    string server_ip;
    sockaddr_in server_addr;

    SOCKET client_socket;

public:

    static User* GetInstance(string name, string password, string ip, int port)
    {
        if (instance == nullptr) 
        {
            instance = new User(name, password, ip, port);
        }

        return instance;
    } 

    string GetName() const 
    {
        return name;
    }

    string GetPassword() const 
    {
        return password;
    }

    void StartChating()
    {
        Init();

        CreateSocket();

        ConnectSocket();

        thread recv_thread = thread(&User::Receive, this);
        thread send_thread = thread(&User::Send, this);

        recv_thread.join();
        send_thread.join();
    }

    ~User()
    {
        if (client_socket != INVALID_SOCKET)
            closesocket(client_socket);

        WSACleanup();
    }


private:

    User(string name, string password, string ip, int port)
        : name(name), password(password), server_ip(ip), wsa{0}, client_socket(INVALID_SOCKET)
    {
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(2222);
        server_addr.sin_addr.s_addr = inet_addr(server_ip.c_str());
    }

    void Init()
    {
        assert(!(WSAStartup(MAKEWORD(2, 2), &wsa)) && "Couldn't init wsa");
        cout << "WSA startup +" << endl;
    }

    void CreateSocket()
    {
        assert(!((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR) && ("Couldn't create socket"));
        cout << "Client socket created + " << endl;
    }

    void ConnectSocket()
    {
        assert(!(connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr))));
        cout << "Connect to server + " << endl;
    }

    void Send()
    {
        string text;

        while (text != "Exit")
        {
            getline(cin, text);

            string message = name + ": " + text;

            assert((send(client_socket, message.c_str(), message.size(), 0)) != -1);

            //cout << "Messages is sent" << endl;
        }

        closesocket(client_socket);

        WSACleanup();

        exit(0);
    }

    void Receive()
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
};

User* User::instance = nullptr;
