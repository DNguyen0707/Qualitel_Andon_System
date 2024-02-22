#include <iostream>
#include <WS2tcpip.h>
#include <string>

//calling winsock library
#pragma comment (lib, "ws2_32.lib")

using namespace std;

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
    hint.sin_port = htons(6000); //port num
    hint.sin_addr.S_un.S_addr = INADDR_ANY;

    bind(listening, (sockaddr*)&hint, sizeof(hint));

    //listening
    listen(listening, SOMAXCONN);
    cout << "listening" << endl;

    //looking for connection
    sockaddr_in client;
    int clientSize = sizeof(client);

    //making rpi socket
    SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "cant make rpi socket" << endl;
        return -1;
    }

    char host[NI_MAXHOST];      //rpi remote name
    char service[NI_MAXSERV];   //port the client is connected on

    ZeroMemory(host, NI_MAXHOST); //memset but for window API
    ZeroMemory(service, NI_MAXSERV);

    //cehck if connection actually works
    if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
        cout << host << " connected on port " << service << endl;
    } else {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        cout << host << " connected on port " << ntohs(client.sin_port) << endl;
    }

    //close server socket
    closesocket(listening);

    //accept and echo message back to client as test
    char buf[4096];

    while (true) {
        ZeroMemory(buf, 4096);

        //wait for rpi to send data
        int bytesReceived = recv(clientSocket, buf, 4096, 0);
        if (bytesReceived == SOCKET_ERROR) {
            cerr << "Error in receiving" << endl;
            break;
        }

        //if not getting anything from rpi
        if (bytesReceived == 0) {
            cout << "rpi disconnected" << endl;
            break;
        }

        cout << buf << endl;

        //resend message back to rpi
        send(clientSocket, buf, bytesReceived + 1, 0);
    }

    //close rpi socket
    closesocket(clientSocket);

    //clean Winsock
    WSACleanup();

    return 0;
}