import socket

serverIP = '10.1.68.62'
serverPort = 6000

#create UDP socket
serverSock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

#bind socket to server address
serverSock.bind((serverIP, serverPort))

print("it's running")

while True:
    #recieve data
    data, clientAddress = serverSock.recvfrom(1024)
    print(f"Got it from {clientAddress}: {data.decode()}")
    
    #send a response back to the client
    response = input("Enter message to send: ")
    serverSock.sendto(response.encode(), clientAddress)