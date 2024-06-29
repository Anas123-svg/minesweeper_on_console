import socket

def main():
    # Server address and port
    server_address = ('localhost', 12345)

    # Create a UDP socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    try:
        # Send data
        message = b'Hello, server!'
        print("Sending:", message)
        sock.sendto(message, server_address)

        # Receive response
        data, server = sock.recvfrom(1020004)
        print("Received:", data.decode())

    except Exception as e:
        print("An error occurred:", e)

    finally:
        # Close the socket
        sock.close()

if __name__ == '__main__':
    main()
