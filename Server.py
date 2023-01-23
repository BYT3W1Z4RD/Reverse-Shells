import socket

IP = "127.0.0.1"
PORT = 8080

# Create the listener
listener = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
listener.bind((IP, PORT))
listener.listen(1)

# Wait for a connection
client, addr = listener.accept()

# Send a message to the client
client.send("New Client Connected".encode())

# Receive commands from the client
while True:
    command = client.recv(1024).decode()
    # Execute the command and send the output back to the client
    output = os.popen(command).read()
    client.send(("PS " + os.getcwd() + "> " + output).encode())
