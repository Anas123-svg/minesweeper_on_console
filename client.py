import socket

server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server_address = ('localhost', 12345)
server_socket.bind(server_address)

print('Waiting for messages...')

while True:
    data, client_address = server_socket.recvfrom(1024)
    filename = data.decode()

    print("Received filename:", filename)

    print('Receiving messages...')
    while True:
        data, client_address = server_socket.recvfrom(1024)
        message = data.decode()
        if not message:
            break
        print("Received message:", message)

server_socket.close() 
