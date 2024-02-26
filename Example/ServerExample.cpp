#include <iostream>
#include <WS2tcpip.h>
#include <string>

//calling winsock library
#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define PORT 6000   //port number
#define MAX_CLIENTS 5   //change this accordingly

int main() {
    //initalize
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);
    int wsOK = WSAStartup(ver, &wsData);
    if (wsOK != 0) {
        cerr << "cant initialize" << endl;
        return -1;
    }

    //make socket
    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == INVALID_SOCKET) {
        cerr << "cant create socket" << endl;
        return -1;
    }

    //bind socket
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(6000);
    hint.sin_addr.S_un.S_addr = INADDR_ANY;

    bind(listening, (sockaddr*)&hint, sizeof(hint));

    //listening
    listen(listening, SOMAXCONN);

    cout << "listening for connection" << endl;

    fd_set master;
    FD_ZERO(&master);
    FD_SET(listening, &master);
    SOCKET clients[MAX_CLIENTS];
    int clientsCount = 0;

    //checking multiple rpi
    while(true)
    {
        fd_set copy = master;
        int socketCount = select(0, &copy, nullptr, nullptr, nullptr);

        for(int i = 0; i < socketCount; i++)
        {
            SOCKET sock = copy.fd_array[i];
            
            if (sock == listening)
            {
                //accept new connect
                SOCKET client = accept(listening, nullptr, nullptr);
                //add new connection to the list of connected
                clients[clientsCount] = client;
                clientsCount++;
                FD_SET(client, &master);
                cout << "connected to new client" << endl;
            }
            else
            {
                //recieve
                char buf[4096];
                ZeroMemory(buf, 4096);
                int bytesIn = recv(sock, buf, 4096, 0);

                //drop client if no data
                if (bytesIn <= 0)
                {
                    closesocket(sock);
                    FD_CLR(sock, &master);
                    cout << "A client have dropped" << endl;
                }
                //echo message and print it out
                else
                {
                    cout << "in echo" << endl;
                    for (int i = 0; i < clientsCount; i++)
                    {
                        //if the client ID same as sock ID
                        if (clients[i] == sock)
                        {
                            cout << buf << endl;
                            send(clients[i], buf, bytesIn + 1, 0);
                            
                        }
                    }
                }
            }
        }
    }

    //clean Winsock
    WSACleanup();

    return 0;
}