import socket

serverIP = '10.1.68.62'
serverPort = 6000

#create UDP socket
clientSock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

while True:
    #sending data
    #get input
    message = input("Enter message to send: ")
    
    clientSock.sendto(message.encode(), (serverIP, serverPort))
    
    #recieve
    respond, serverAddress = clientSock.recvfrom(1024)
    print(f"Got it from {serverAddress}: {respond.decode()}")
    