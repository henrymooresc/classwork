from socket import *
from struct import pack, unpack

HEADER_SIZE = 4

class BufferedTCPEchoServer(object):
    
    def __init__(self, host = '', port = 36001, buffer_size = 1024):
        self.buffer_size = buffer_size

        # This variable is used to tell the server when it should shut down.
        self.keep_running = True

        # Creates and binds the server socket
        self.sock = socket(AF_INET, SOCK_STREAM)
        self.sock.bind((host, port))

    # Listens for new connections and new messages. Calls helper functions to handle receiving and sending data
    def start(self):
        print('SERVER: listening...')

        # Loops until keep_running is set to False
        while self.keep_running:

            # Listens for and accepts new connections
            self.sock.listen(1)
            self.connSock, self.connAddr = self.sock.accept()
            is_connected = True
            print("SERVER: New connection made")

            # Looks for messages until the client disconnects
            while is_connected:
                is_connected = self.receive_message()

            # Closes the socket for disconnected client
            print("SERVER: Closing socket for client " + self.connAddr[0])
            self.connSock.close()

        self.shutdown()

    # Receives a message from the client connection and passes it into the send_message function
    # Returns a bool to indicate if the interaction was successful
    def receive_message(self):
        print("SERVER: Attempting to receieve...")

        # Recv call wrapped in try/except for handling an abrupt disconnect
        try:
            data = self.connSock.recv(HEADER_SIZE)

            # Ensuring data was actually received
            if data:
                length = unpack("!I", data)[0]

                payload = ""

                # Looping to ensure the entire message is received if it is larger than the buffer size
                while len(payload) < length:

                    # Lowers the buffer size to ensure the recv call doesn't grab data for a different message
                    buffSize = min(self.buffer_size, length - len(payload))
                    data = self.connSock.recv(buffSize)

                    if data:
                        payload += data.decode()

                    else:
                        print("SERVER: Empty byte array, receive failed...")
                        return False

                # Passes the message on to be sliced and sent back to the client
                return self.send_message(payload)

            # If an empty byte array is returned, it indicates the connection was closed
            else:
                print("SERVER: Empty byte array, receive failed...")
                return False
        
        except ConnectionResetError:
            print("SERVER: Connection reset error, receive failed...")
            return False

    # Accepts a message and sends a modified version to the client
    # Returns a bool to indicate if the send was successful
    def send_message(self, message):

        # Slices off the first 10 characters as specified
        message = message[10:]

        # Packing the message for transmission
        messageLength = len(message)
        data = pack("!I" + str(messageLength) + "s", messageLength, message.encode())

        print("SERVER: Attempting to send response...")

        # Send call wrapped in try/except to handle abrupt disconnects
        try:
            self.connSock.send(data)
            return True

        except ConnectionResetError:
            print("SERVER: Connection reset error, send failed...")
            return False

    # Closes the server socket
    def shutdown(self):
        print("SERVER: shutting down...")
        self.sock.close()


if __name__ == "__main__":
    BufferedTCPEchoServer(host='', port=36001, buffer_size=1024).start()