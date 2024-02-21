#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

int main()
{
    //declar variables
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientLen = sizeof(clientAddr);
    char buffer[1024];

    //create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
    {
        cerr << "Error in socket creation\n";
        return -1;
    }

    //bind socket to port
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(6000);  //port number

    //check for bind error
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) , 0)
    {
        cerr << "Error in binding\n";
        return -1;
    }

    //listen for incoming
    listen(serverSocket, 5);
    cout << "Listening...\n"

    //accept connection
    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientLen);
    if (clientSocket < 0)
    {
        cerr << "error in accepting connection\n"
        return -1;
    }

    cout << "Connection established with client\n";
    
    //received data
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived < 0)
    {
        cerr << "Error in receiving data\n";
        return -1;
    }

    //close sockets
    close(clientSocket);
    close(serverSocket);

    return 0;
}