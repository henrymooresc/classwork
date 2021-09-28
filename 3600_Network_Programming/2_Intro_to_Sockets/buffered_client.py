from socket import *
from struct import pack, unpack

HEADER_SIZE = 4

class BufferedTCPClient:

    def __init__(self, server_host='localhost', server_port=36001, buffer_size=1024):
        self.buffer_size = buffer_size

        # Creates the socket and connects it to specified server
        self.sock = socket(AF_INET, SOCK_STREAM)
        self.sock.connect((server_host, server_port))


    # Accepts a message and sends it to the connected server.
    def send_message(self, message):
        print("CLIENT: Attempting to send a message...")
        
        messageLength = len(message)

        data = pack("!I" + str(messageLength) + "s", messageLength, message.encode())

        # Send call wrapped in try/except for handling an abrupt disconnect
        try:
            self.sock.send(data)

        except ConnectionResetError:
            print("CLIENT: Connection reset error, send failed...")


    # Waits to receieve a message from the socket from the server.
    # Returns the message received and a bool indicating if the receive was successful. If not successful it returns an empty string
    def receive_message(self):
        print("CLIENT: Attempting to receive a message...")

        # Recv call wrapped in try/except for handling an abrupt disconnect
        try:
            data = self.sock.recv(HEADER_SIZE)

            # Ensuring data was actually received
            if data:
                length = unpack("!I", data)[0]

                payload = ""

                # Looping to ensure the entire message is received if it is larger than the buffer size
                while len(payload) < length:

                    # Lowers the buffer size to ensure the recv call doesn't grab data for a different message
                    buffSize = min(self.buffer_size, length - len(payload))
                    data = self.sock.recv(buffSize)

                    if data:
                        payload += data.decode()

                    else:
                        print("CLIENT: Empty byte array, receive failed...")
                        return "", False

                return payload, True

            # If an empty byte array is returned, it indicates the connection was closed
            else:
                print("CLIENT: Empty byte array, receive failed...")
                return "", False
        
        except ConnectionResetError:
            print("CLIENT: Connection reset error, receive failed...")
            return "", False


    # Closes the client socket
    def shutdown(self):
        print("Client: Attempting to shut down...")
        self.sock.close()

        
if __name__ == "__main__":
    l = BufferedTCPClient(server_host="localhost", server_port=36001)

    l.send_message("Four score and seven years ago")
    response = l.receive_message()
    print(response)