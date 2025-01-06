#pragma once
#include <iostream>
#include <string>
#include <winsock2.h>

using namespace std;

class TcpSocketServer {
    public:
        TcpSocketServer(): serverSocket(0), serverAddress({0}), clientSocket(0), clientAddress({0}), len(0) {
            // Initialize Socket
            WSADATA data;
            WORD version = MAKEWORD(2, 2);
            if (WSAStartup(version, &data) != 0) {
                cerr << "Socket startup failed" << endl;
            }
        }

        int Bind(u_short port, string ip = "") {
            // Create a socket
            serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            // Initialize ip address information
            serverAddress.sin_family = AF_INET;
            serverAddress.sin_port = htons(port);
            if (ip.size() == 0) {
                serverAddress.sin_addr.s_addr = INADDR_ANY;
            } else {
                serverAddress.sin_addr.s_addr = inet_addr(ip.c_str());

            }
            // Binding process
            return bind(serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress));
        }

        int Listen(int num) {
            return listen(serverSocket, num);
        }

        SOCKET Accept() {
            len = sizeof(clientAddress);
            clientSocket = accept(serverSocket, (sockaddr*)&clientAddress, &len);
            return clientSocket;
        }

        string GetClientIP() const {
            return inet_ntoa(clientAddress.sin_addr);
        }

        int SendData(string message) {
            return send(clientSocket, message.c_str(), message.size() + 1, 0);
        }

        ~TcpSocketServer() {
            closesocket(serverSocket);
            closesocket(clientSocket);
            WSACleanup();
        }

    protected:
        // Socket and IP address
        SOCKET serverSocket;
        sockaddr_in serverAddress;

        SOCKET clientSocket;
        sockaddr_in clientAddress;
        int len;
};