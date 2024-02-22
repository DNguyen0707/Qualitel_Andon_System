#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//linux packages
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

using namespace std;

int main() {
    //create socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        cerr << "cant create socket" << endl;
        return -1;
    }

    //declaring port and ip
    int port = 6000; //port
    string ipAddress = "10.1.68.62"; //server ip (my pc for the time being)

    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &server.sin_addr);

    // Connect to the server on the socket
    int connectRes = connect(sock, (sockaddr*)&server, sizeof(server));
    if (connectRes == -1) {
        cerr << "cant connect to server" << endl;
        return -1;
    }

    
    char buf[4096];
    string userInput;

    do {
        //user input as test
        cout << "> ";
        getline(cin, userInput);

        //send
        int sendRes = send(sock, userInput.c_str(), userInput.size() + 1, 0);
        if (sendRes == -1) {
            cout << "cant send to server" << endl;
            continue;
        }

        //wait for respodn from server
        memset(buf, 0, 4096);
        int bytesReceived = recv(sock, buf, 4096, 0);
        if (bytesReceived == -1) {
            cerr << "error getting response" << endl;
        } else {
            //display
            cout << "SERVER> " << string(buf, bytesReceived) << endl;
        }

    } while(true);

    //close socket
    close(sock);

    return 0;
}