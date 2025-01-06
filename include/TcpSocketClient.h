#pragma once
#include <iostream>
#include <string>
#include <winsock2.h>

using namespace std;

class TcpSocketClient {
    public:
        TcpSocketClient(): serverSocket(0), serverAddress({0}), clientSocket(0), clientAddress({0}), len(0) {
            // Initialize Socket
            WSADATA data;
            WORD version = MAKEWORD(2, 2);
            if (WSAStartup(version, &data) != 0) {
                cerr << "Socket startup failed" << endl;
            }
        }

        int Request(u_short port, string ip = "") {
            // Create a socket
            serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            // Initialize ip address information
            serverAddress.sin_family = AF_INET;
            serverAddress.sin_port = htons(port);
            if (ip.size() == 0) {
                serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
            } else {
                serverAddress.sin_addr.s_addr = inet_addr(ip.c_str());

            }
            return connect(serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress));
        }

        string ReceiveData(int messageLen) {
            char* message = new char[messageLen];
            memset(message, 0, messageLen);
            recv(serverSocket, message, messageLen, 0);
            string data = string(message);
            delete[] message;
            return data;
        }


        ~TcpSocketClient() {
            closesocket(serverSocket);
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