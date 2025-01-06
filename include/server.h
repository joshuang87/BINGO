#pragma once
#include "TcpSocketServer.h"

void startSever() {}

void testServer() {
    TcpSocketServer server;
    server.Bind(1588);
    server.Listen(2);
    cout << "Server started" << endl;
    while (1)
    {
        if (server.Accept() != INVALID_SOCKET) {
            cout << server.GetClientIP() << " : Client connected" << endl;
            server.SendData("Hello from server");
            
            if (getchar()) {
                break;
            }

        }
    }
}